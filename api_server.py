import json
from copy import deepcopy
from http import HTTPStatus
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path


BASE_DIR = Path(__file__).resolve().parent
HOST = "127.0.0.1"
PORT = 8000
BASE_CAPACITY = 50
MIN_GREEN = 15
MAX_GREEN = 60
FIXED_TIME_DURATION = 30
MAX_TIMELINE_ENTRIES = 5

SCENARIOS = {
    "MORNING_RUSH": {
        "label": "Morning Rush",
        "detail": "High inbound load from North and East",
        "lanes": {
            "North": {"vehicles": 9, "wait": 18},
            "South": {"vehicles": 3, "wait": 10},
            "East": {"vehicles": 8, "wait": 16},
            "West": {"vehicles": 2, "wait": 8},
        },
    },
    "MIDDAY": {
        "label": "Midday",
        "detail": "Balanced moderate traffic on all approaches",
        "lanes": {
            "North": {"vehicles": 5, "wait": 12},
            "South": {"vehicles": 4, "wait": 11},
            "East": {"vehicles": 5, "wait": 12},
            "West": {"vehicles": 4, "wait": 10},
        },
    },
    "EVENING_RUSH": {
        "label": "Evening Rush",
        "detail": "Heavy outbound load on South and West",
        "lanes": {
            "North": {"vehicles": 2, "wait": 7},
            "South": {"vehicles": 9, "wait": 18},
            "East": {"vehicles": 3, "wait": 8},
            "West": {"vehicles": 8, "wait": 17},
        },
    },
    "NIGHT": {
        "label": "Night",
        "detail": "Light demand with sparse arrivals and short queues",
        "lanes": {
            "North": {"vehicles": 1, "wait": 4},
            "South": {"vehicles": 1, "wait": 4},
            "East": {"vehicles": 0, "wait": 0},
            "West": {"vehicles": 1, "wait": 3},
        },
    },
}


class TrafficSimulationAPI:
    def __init__(self) -> None:
        self.state = {
            "scenario": "MORNING_RUSH",
            "emergencySouth": False,
            "blockageEast": False,
            "lanes": {},
            "selectedLane": "North",
            "greenTime": 28,
            "processed": 0,
            "cycle": 0,
            "history": [],
        }
        self.reset()

    def clone_scenario(self, name: str) -> dict:
        return deepcopy(SCENARIOS[name]["lanes"])

    def effective_capacity(self, lane_name: str) -> int:
        if lane_name == "East" and self.state["blockageEast"]:
            return BASE_CAPACITY // 2
        return BASE_CAPACITY

    def compute_priority(self, lane_name: str, lane_data: dict) -> float:
        if lane_name == "South" and self.state["emergencySouth"]:
            return 999999.0
        capacity = self.effective_capacity(lane_name)
        if capacity == 0:
            return 0.0
        return (lane_data["vehicles"] * lane_data["wait"]) / capacity

    def calculate_green_time(self, vehicle_count: int) -> int:
        raw = 10 + vehicle_count * 2
        return max(MIN_GREEN, min(MAX_GREEN, raw))

    def choose_lane(self) -> str:
        ranked = sorted(
            self.state["lanes"].items(),
            key=lambda item: (
                self.compute_priority(item[0], item[1]),
                item[1]["vehicles"],
            ),
            reverse=True,
        )
        return ranked[0][0]

    def total_vehicles(self) -> int:
        return sum(lane["vehicles"] for lane in self.state["lanes"].values())

    def average_wait(self) -> float:
        total_vehicles = self.total_vehicles()
        if total_vehicles == 0:
            return 0.0
        weighted_wait = sum(
            lane["vehicles"] * lane["wait"] for lane in self.state["lanes"].values()
        )
        return weighted_wait / total_vehicles

    def greedy_estimate(self) -> int:
        return self.state["greenTime"] // 2

    def fixed_estimate(self) -> int:
        return FIXED_TIME_DURATION // 2

    def pressure_level(self, priority: float) -> str:
        if priority == 999999.0:
            return "Emergency"
        if priority >= 3:
            return "Critical"
        if priority >= 1.6:
            return "Elevated"
        if priority >= 0.5:
            return "Stable"
        return "Light"

    def lane_status(self, lane_name: str, priority: float) -> str:
        if lane_name == "South" and self.state["emergencySouth"]:
            return "Emergency"
        if lane_name == "East" and self.state["blockageEast"]:
            return "Constrained"
        if lane_name == self.state["selectedLane"]:
            return "Allocated"
        return self.pressure_level(priority)

    def scenario_explanation(self) -> str:
        detail = SCENARIOS[self.state["scenario"]]["detail"]
        if self.state["emergencySouth"]:
            return (
                "South is carrying an emergency vehicle, so the controller "
                "suspends normal ranking and grants immediate right of way "
                "ahead of all other approaches."
            )
        if self.state["blockageEast"]:
            return (
                f"{detail}. East capacity is reduced to 25 vehicles, which raises "
                "congestion pressure faster and changes the balance of the next allocation."
            )
        return (
            f"{detail}. {self.state['selectedLane']} now leads the allocation stack "
            "because its queue depth and waiting time create the highest pressure score."
        )

    def alert_text(self) -> str:
        if self.state["emergencySouth"]:
            return "Emergency override active on South"
        if self.state["blockageEast"]:
            return "East approach operating under reduced capacity"
        return "No active incident"

    def network_state_text(self) -> str:
        if self.state["cycle"] == 0:
            return "Priority controller active"
        return f"Priority controller active after {self.state['cycle']} simulated cycles"

    def update_state(self) -> None:
        lane_name = self.choose_lane()
        self.state["selectedLane"] = lane_name
        self.state["greenTime"] = self.calculate_green_time(
            self.state["lanes"][lane_name]["vehicles"]
        )

    def record_cycle(self, cleared: int) -> None:
        lane_data = self.state["lanes"][self.state["selectedLane"]]
        self.state["history"].insert(
            0,
            {
                "cycle": self.state["cycle"],
                "lane": self.state["selectedLane"],
                "cleared": cleared,
                "wait": lane_data["wait"],
                "green": self.state["greenTime"],
            },
        )
        self.state["history"] = self.state["history"][:MAX_TIMELINE_ENTRIES]

    def reset(self) -> None:
        self.state["lanes"] = self.clone_scenario(self.state["scenario"])
        self.state["processed"] = 0
        self.state["cycle"] = 0
        self.state["history"] = []
        self.update_state()

    def set_scenario(self, scenario: str) -> None:
        if scenario not in SCENARIOS:
            raise ValueError("Unknown scenario")
        self.state["scenario"] = scenario
        self.reset()

    def set_toggles(self, emergency_south=None, blockage_east=None) -> None:
        if emergency_south is not None:
            self.state["emergencySouth"] = bool(emergency_south)
        if blockage_east is not None:
            self.state["blockageEast"] = bool(blockage_east)
        self.update_state()

    def run_cycle(self) -> None:
        self.update_state()
        selected_lane = self.state["selectedLane"]
        lane = self.state["lanes"][selected_lane]
        cleared = min(self.greedy_estimate(), lane["vehicles"])
        lane["vehicles"] -= cleared
        lane["wait"] = max(0, lane["wait"] - (self.state["greenTime"] // 3))
        self.state["processed"] += cleared

        for lane_name, lane_data in self.state["lanes"].items():
            if lane_name == selected_lane:
                continue

            if self.state["scenario"] == "MORNING_RUSH" and lane_name in {"North", "East"}:
                arrival_boost = 2
            elif self.state["scenario"] == "EVENING_RUSH" and lane_name in {"South", "West"}:
                arrival_boost = 2
            elif self.state["scenario"] == "NIGHT":
                arrival_boost = 0
            else:
                arrival_boost = 1

            lane_data["vehicles"] += arrival_boost
            if lane_data["vehicles"] > 0:
                lane_data["wait"] += 2 + (1 if arrival_boost > 1 else 0)

        self.state["cycle"] += 1
        self.update_state()
        self.record_cycle(cleared)

    def run_batch(self, cycles: int) -> None:
        if cycles <= 0:
            raise ValueError("Cycle count must be positive")
        for _ in range(cycles):
            self.run_cycle()

    def payload(self) -> dict:
        lanes = {}
        for lane_name, lane_data in self.state["lanes"].items():
            priority = self.compute_priority(lane_name, lane_data)
            capacity = self.effective_capacity(lane_name)
            lanes[lane_name] = {
                "vehicles": lane_data["vehicles"],
                "wait": lane_data["wait"],
                "priority": priority,
                "capacity": capacity,
                "status": self.lane_status(lane_name, priority),
            }

        return {
            "scenarios": {
                key: {"label": value["label"], "detail": value["detail"]}
                for key, value in SCENARIOS.items()
            },
            "state": {
                **self.state,
                "averageWait": round(self.average_wait(), 1),
                "greedyEstimate": self.greedy_estimate(),
                "fixedEstimate": self.fixed_estimate(),
                "networkState": self.network_state_text(),
                "networkAlert": self.alert_text(),
                "explanation": self.scenario_explanation(),
                "lanes": lanes,
            },
        }


SIMULATION = TrafficSimulationAPI()


class TrafficRequestHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(BASE_DIR), **kwargs)

    def do_GET(self) -> None:
        if self.path == "/api/state":
            self.respond_json(SIMULATION.payload())
            return
        super().do_GET()

    def do_POST(self) -> None:
        if not self.path.startswith("/api/"):
            self.respond_json({"error": "Not found"}, status=HTTPStatus.NOT_FOUND)
            return

        try:
            payload = self.read_json()

            if self.path == "/api/scenario":
                SIMULATION.set_scenario(payload["scenario"])
            elif self.path == "/api/toggles":
                SIMULATION.set_toggles(
                    emergency_south=payload.get("emergencySouth"),
                    blockage_east=payload.get("blockageEast"),
                )
            elif self.path == "/api/run-cycle":
                SIMULATION.run_cycle()
            elif self.path == "/api/run-batch":
                SIMULATION.run_batch(int(payload.get("cycles", 0)))
            elif self.path == "/api/reset":
                SIMULATION.reset()
            else:
                self.respond_json({"error": "Not found"}, status=HTTPStatus.NOT_FOUND)
                return

            self.respond_json(SIMULATION.payload())
        except KeyError as exc:
            self.respond_json(
                {"error": f"Missing field: {exc.args[0]}"},
                status=HTTPStatus.BAD_REQUEST,
            )
        except ValueError as exc:
            self.respond_json({"error": str(exc)}, status=HTTPStatus.BAD_REQUEST)

    def read_json(self) -> dict:
        content_length = int(self.headers.get("Content-Length", "0"))
        if content_length == 0:
            return {}
        raw = self.rfile.read(content_length).decode("utf-8")
        return json.loads(raw)

    def respond_json(self, payload: dict, status: HTTPStatus = HTTPStatus.OK) -> None:
        body = json.dumps(payload).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)


def run() -> None:
    server = ThreadingHTTPServer((HOST, PORT), TrafficRequestHandler)
    print(f"Serving Smart Traffic Signal Optimisation at http://{HOST}:{PORT}")
    server.serve_forever()


if __name__ == "__main__":
    run()
