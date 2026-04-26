const state = {
  scenarios: {},
  data: null,
  busy: false
};

function setBusy(isBusy) {
  state.busy = isBusy;
  document.getElementById("run-cycle").disabled = isBusy;
  document.getElementById("reset-state").disabled = isBusy;
  document.getElementById("priority-pulse").disabled = isBusy;
  document.getElementById("emergency-mode").disabled = isBusy;
  document.getElementById("capacity-test").disabled = isBusy;
}

async function request(path, payload = null) {
  setBusy(true);

  try {
    const response = await fetch(path, {
      method: payload ? "POST" : "GET",
      headers: {
        "Content-Type": "application/json"
      },
      body: payload ? JSON.stringify(payload) : undefined
    });

    const data = await response.json();
    if (!response.ok) {
      throw new Error(data.error || "Request failed");
    }

    state.scenarios = data.scenarios;
    state.data = data.state;
    render();
    return data;
  } catch (error) {
    console.error(error);
    alert(error.message);
  } finally {
    setBusy(false);
  }
}

async function runPriorityPulse() {
  if (state.busy) {
    return;
  }

  for (let index = 0; index < 3; index += 1) {
    await request("/api/run-cycle", {});
  }
}

function renderScenarioButtons() {
  const container = document.getElementById("scenario-grid");
  container.innerHTML = "";

  Object.entries(state.scenarios).forEach(([key, value]) => {
    const button = document.createElement("button");
    button.className = `scenario-button${state.data.scenario === key ? " active" : ""}`;
    button.innerHTML = `<strong>${value.label}</strong><span>${value.detail}</span>`;
    button.addEventListener("click", () => request("/api/scenario", { scenario: key }));
    container.appendChild(button);
  });
}

function renderLanes() {
  Object.entries(state.data.lanes).forEach(([name, lane]) => {
    const card = document.querySelector(`.lane-card[data-lane="${name}"]`);
    const badge = document.getElementById(`badge-${name}`);
    const countEl = document.getElementById(`count-${name}`);
    const metaEl = document.getElementById(`meta-${name}`);
    const barEl = document.getElementById(`bar-${name}`);

    countEl.textContent = lane.vehicles;
    metaEl.textContent = `Wait ${lane.wait}s | Priority ${lane.priority === 999999 ? "Emergency" : lane.priority.toFixed(2)} | Cap ${lane.capacity}`;
    badge.textContent = lane.status;
    barEl.style.width = `${Math.min((lane.vehicles / lane.capacity) * 100, 100)}%`;

    card.classList.toggle("selected", state.data.selectedLane === name);
    card.classList.toggle("alert", lane.status === "Emergency");
    card.classList.toggle("blocked", lane.status === "Constrained");
  });
}

function renderAnalytics() {
  const scenario = state.scenarios[state.data.scenario];

  document.getElementById("active-pattern").textContent = scenario.label;
  document.getElementById("pattern-detail").textContent = scenario.detail;
  document.getElementById("selected-lane").textContent = state.data.selectedLane;
  document.getElementById("green-time").textContent = `${state.data.greenTime}s`;
  document.getElementById("signal-caption").textContent = `Green corridor assigned to ${state.data.selectedLane}`;
  document.getElementById("signal-core-text").textContent = `${state.data.selectedLane} corridor has green priority`;
  document.getElementById("processed-count").textContent = String(state.data.processed);
  document.getElementById("average-wait").textContent = `${state.data.averageWait.toFixed(1)}s`;
  document.getElementById("greedy-estimate").textContent = `${state.data.greedyEstimate} vehicles`;
  document.getElementById("fixed-estimate").textContent = `${state.data.fixedEstimate} vehicles`;
  document.getElementById("network-state").textContent = state.data.networkState;
  document.getElementById("network-alert").textContent = state.data.networkAlert;
  document.getElementById("explanation").textContent = state.data.explanation;

  document.getElementById("emergency-toggle").checked = state.data.emergencySouth;
  document.getElementById("blockage-toggle").checked = state.data.blockageEast;
  document.getElementById("emergency-mode").classList.toggle("active", state.data.emergencySouth);
  document.getElementById("capacity-test").classList.toggle("active", state.data.blockageEast);
}

function renderTimeline() {
  const timeline = document.getElementById("timeline");

  if (state.data.history.length === 0) {
    timeline.innerHTML = `
      <div class="timeline-item">
        <span class="timeline-step">0</span>
        <div class="timeline-copy">
          <strong>Ready state</strong>
          <span>Advance a cycle to capture the first allocation record.</span>
        </div>
        <span class="timeline-meta">Standby</span>
      </div>
    `;
    return;
  }

  timeline.innerHTML = state.data.history.map((entry) => `
    <div class="timeline-item">
      <span class="timeline-step">${entry.cycle}</span>
      <div class="timeline-copy">
        <strong>${entry.lane} allocated</strong>
        <span>Cleared ${entry.cleared} vehicles with a ${entry.green}s green window.</span>
      </div>
      <span class="timeline-meta">Wait ${entry.wait}s</span>
    </div>
  `).join("");
}

function renderLaneTable() {
  const container = document.getElementById("lane-table");
  const head = `
    <div class="lane-table-head">
      <span>Lane</span>
      <span>Queue</span>
      <span>Average wait</span>
      <span>Priority</span>
      <span>Capacity</span>
      <span>Status</span>
    </div>
  `;

  const rows = Object.entries(state.data.lanes).map(([name, lane]) => {
    const statusClass =
      lane.status === "Emergency" ? "alert" :
      lane.status === "Constrained" ? "blocked" :
      lane.status === "Allocated" ? "selected" : "";

    return `
      <div class="lane-table-row">
        <div class="table-main">
          <strong>${name}</strong>
          <span>${lane.status === "Allocated" ? "Current green allocation" : "Awaiting allocation"}</span>
        </div>
        <div class="table-cell">${lane.vehicles} vehicles</div>
        <div class="table-cell">${lane.wait}s</div>
        <div class="table-cell">${lane.priority === 999999 ? "Emergency" : lane.priority.toFixed(2)}</div>
        <div class="table-cell">${lane.capacity}</div>
        <div class="table-cell"><span class="table-status ${statusClass}">${lane.status}</span></div>
      </div>
    `;
  }).join("");

  container.innerHTML = head + rows;
}

function render() {
  renderScenarioButtons();
  renderLanes();
  renderAnalytics();
  renderTimeline();
  renderLaneTable();
}

document.getElementById("emergency-toggle").addEventListener("change", (event) => {
  request("/api/toggles", { emergencySouth: event.target.checked });
});

document.getElementById("blockage-toggle").addEventListener("change", (event) => {
  request("/api/toggles", { blockageEast: event.target.checked });
});

document.getElementById("run-cycle").addEventListener("click", () => {
  request("/api/run-cycle", {});
});

document.getElementById("reset-state").addEventListener("click", () => {
  request("/api/reset", {});
});

document.getElementById("priority-pulse").addEventListener("click", () => {
  runPriorityPulse();
});

document.getElementById("emergency-mode").addEventListener("click", () => {
  request("/api/toggles", { emergencySouth: !state.data.emergencySouth });
});

document.getElementById("capacity-test").addEventListener("click", () => {
  request("/api/toggles", { blockageEast: !state.data.blockageEast });
});

request("/api/state");
