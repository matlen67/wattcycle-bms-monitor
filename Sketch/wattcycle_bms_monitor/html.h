const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>BMS Monitor</title>
    <style>
        :root {
            --bg-dark: #0f172a;
            --bg-card: #1e293b;
            --text-main: #f8fafc;
            --text-muted: #94a3b8;
            --accent-blue: #38bdf8;
            --color-success: #4ade80;
            --color-warning: #fbbf24;
            --color-danger: #f87171;
            --border-color: #334155;
            --font-family: 'Segoe UI', Roboto, Helvetica, Arial, sans-serif;
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
        }

        body {
            background-color: var(--bg-dark);
            color: var(--text-main);
            font-family: var(--font-family);
            display: flex;
            min-height: 100vh;
        }

        /* Layout mit Sidebar */
        .bms-layout {
            display: flex;
            width: 100%;
        }

        /* Sidebar Navigation */
        .sidebar {
            width: 240px;
            background-color: var(--bg-card);
            border-right: 1px solid var(--border-color);
            display: flex;
            flex-direction: column;
            padding: 20px;
            flex-shrink: 0;
        }

        .logo-area {
            display: flex;
            align-items: center;
            gap: 12px;
            margin-bottom: 30px;
            padding-bottom: 15px;
            border-bottom: 1px solid var(--border-color);
        }

        .status-indicator {
            width: 12px;
            height: 12px;
            border-radius: 50%;
        }
        .status-indicator.online { 
            background-color: var(--color-success); 
            box-shadow: 0 0 10px var(--color-success); 
        }

        .badge {
            background-color: var(--accent-blue);
            color: var(--bg-dark);
            font-size: 0.7rem;
            padding: 2px 6px;
            border-radius: 4px;
            vertical-align: middle;
        }

        .nav-menu {
            display: flex;
            flex-direction: column;
            gap: 10px;
            list-style: none;
        }

        .nav-btn {
            background: none;
            border: none;
            color: var(--text-muted);
            padding: 12px 15px;
            text-align: left;
            font-size: 1rem;
            cursor: pointer;
            border-radius: 8px;
            width: 100%;
            transition: all 0.2s;
            font-weight: 500;
        }

        .nav-btn:hover, .nav-btn.active {
            background-color: var(--border-color);
            color: var(--text-main);
        }
        
        .nav-btn.active {
            border-left: 4px solid var(--accent-blue);
        }

        /* Hauptinhalt */
        .main-content {
            flex-grow: 1;
            padding: 30px;
            overflow-y: auto;
            max-width: 1400px;
        }

        .bms-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 25px;
            padding-bottom: 15px;
            border-bottom: 1px solid var(--border-color);
        }

        .system-time {
            color: var(--text-muted);
            font-size: 0.95rem;
        }

        /* Tab-Sichtbarkeit */
        .tab-content {
            display: none;
        }

        .tab-content.active {
            display: block;
        }

        /* Dashboard Grid Layout (Oberer Teil) */
        .bms-grid-top {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(320px, 1fr));
            gap: 20px;
            margin-bottom: 20px;
        }

        /* Card-Design */
        .card {
            background-color: var(--bg-card);
            border: 1px solid var(--border-color);
            border-radius: 12px;
            padding: 20px;
            box-shadow: 0 4px 6px -1px rgb(0 0 0 / 0.1);
        }

        .card h3 {
            font-size: 1.05rem;
            color: var(--text-muted);
            margin-bottom: 15px;
            font-weight: 600;
            text-transform: uppercase;
            letter-spacing: 0.05em;
        }

        /* SoC-Kreis-Anzeige */
        .soc-display {
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 10px 0;
        }

        .progress-ring {
            position: relative;
            width: 140px;
            height: 140px;
        }

        .progress-ring svg {
            transform: rotate(-90deg);
        }

        .ring-bg {
            fill: transparent;
            stroke: var(--border-color);
            stroke-width: 10;
        }

        .ring-fill {
            fill: transparent;
            stroke: var(--color-success);
            stroke-width: 10;
            stroke-dasharray: 377;
            stroke-linecap: round;
        }

        .soc-value {
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            font-size: 2.2rem;
            font-weight: 700;
        }

        .soc-value span {
            font-size: 1.2rem;
            color: var(--text-muted);
        }

        .soc-footer {
            text-align: center;
            margin-top: 10px;
            font-size: 0.9rem;
        }

        /* Systemparameter Grid */
        .metrics-grid {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 15px;
        }

        .metric-item {
            background-color: rgba(15, 23, 42, 0.4);
            padding: 15px;
            border-radius: 8px;
            border-left: 3px solid var(--accent-blue);
        }

        .metric-item .label {
            display: block;
            font-size: 0.8rem;
            color: var(--text-muted);
            margin-bottom: 5px;
        }

        .metric-item .value {
            font-size: 1.4rem;
            font-weight: bold;
        }

        /* Temperaturen und Alarme */
        .temp-section {
            display: flex;
            gap: 15px;
            margin-bottom: 15px;
        }

        .temp-box {
            flex: 1;
            background: rgba(15, 23, 42, 0.2);
            padding: 12px;
            border-radius: 8px;
            text-align: center;
            border: 1px solid var(--border-color);
        }

        .temp-val { display: block; font-size: 1.2rem; font-weight: bold; }
        .temp-lbl { font-size: 0.75rem; color: var(--text-muted); }

        .alerts-section h4 {
            font-size: 0.85rem;
            color: var(--text-muted);
            margin-bottom: 8px;
        }

        .alert-list {
            list-style: none;
            display: flex;
            flex-direction: column;
            gap: 6px;
        }

        .alert-item {
            font-size: 0.8rem;
            padding: 6px 10px;
            border-radius: 4px;
            border-left: 4px solid transparent;
        }
        .alert-item.info { background: rgba(251, 191, 36, 0.1); color: var(--color-warning); border-left-color: var(--color-warning); }
        .alert-item.success { background: rgba(74, 222, 128, 0.1); color: var(--color-success); border-left-color: var(--color-success); }

        /* 16S Zellen-Sektion (Unten, 2x8 Spalten) */
        .cells-full-width {
            margin-top: 20px;
        }

        .cells-grid-2x8 {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 20px;
        }

        .cell-column {
            display: flex;
            flex-direction: column;
            gap: 10px;
        }

        .cell-row {
            display: flex;
            align-items: center;
            gap: 10px;
            font-size: 0.85rem;
            background: rgba(15, 23, 42, 0.2);
            padding: 6px 10px;
            border-radius: 6px;
            border: 1px solid rgba(51, 65, 85, 0.5);
        }

        .cell-name { width: 60px; color: var(--text-muted); }
        .cell-volt { width: 65px; text-align: right; font-family: monospace; font-weight: bold; }

        .cell-bar-wrapper {
            flex-grow: 1;
            background-color: var(--border-color);
            height: 10px;
            border-radius: 50px;
            overflow: hidden;
        }

        .cell-bar-fill {
            height: 100%;
            border-radius: 5px;
			/* Berechnet den Prozentwert nun mathematisch direkt im CSS auf Basis von Millivolt! */
            /* Formel: ((mV - Min) / (Max - Min)) * 100 */
            /* Hier: ((var(--cell-mv) - 2500) / (3650 - 2500)) * 100 % = ((var(--cell-mv) - 2500) / 150) * 100 % */
            width: clamp(0%, calc((var(--cell-mv) - 2500) / (3650 - 2500) * 100%), 100%);
            transition: width 0.3s ease, background-color 0.3s ease;
        }
        .cell-bar-fill.standard { background-color: var(--accent-blue); }
        .cell-bar-fill.balancing { background-color: var(--color-success); }
        .cell-bar-fill.danger { background-color: var(--color-danger); }

        .cell-summary {
            margin-top: 15px;
            font-size: 0.85rem;
            border-top: 1px solid var(--border-color);
            padding-top: 10px;
            display: flex;
            justify-content: space-between;
        }

        /* Konfigurationsformulare */
        .config-layout {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(350px, 1fr));
            gap: 25px;
        }

        .form-group {
            margin-bottom: 15px;
        }

        .form-group label {
            display: block;
            font-size: 0.85rem;
            color: var(--text-muted);
            margin-bottom: 6px;
        }

        .form-group input, .form-group select {
            width: 100%;
            background-color: var(--bg-dark);
            border: 1px solid var(--border-color);
            color: var(--text-main);
            padding: 10px 12px;
            border-radius: 6px;
            font-size: 0.95rem;
            outline: none;
            transition: border-color 0.2s;
        }

        .form-group input:focus, .form-group select:focus {
            border-color: var(--accent-blue);
        }

        .btn {
            background-color: var(--accent-blue);
            color: var(--bg-dark);
            border: none;
            padding: 12px 20px;
            border-radius: 6px;
            font-size: 1rem;
            font-weight: 600;
            cursor: pointer;
            transition: opacity 0.2s;
            width: 100%;
        }

        .btn:hover {
            opacity: 0.9;
        }

        /* System-Aktionen Zeile */
        .system-actions-row {
            grid-column: 1 / -1;
            display: flex;
            gap: 20px;
            margin-top: 10px;
            padding-top: 20px;
            border-top: 1px solid var(--border-color);
        }

        .btn-system {
            flex: 1;
            padding: 12px 20px;
            border-radius: 6px;
            font-size: 1rem;
            font-weight: 600;
            cursor: pointer;
            border: 1px solid var(--border-color);
            transition: all 0.2s;
            text-align: center;
        }

        .btn-update {
            background-color: rgba(56, 189, 248, 0.1);
            color: var(--accent-blue);
            border-color: var(--accent-blue);
        }

        .btn-update:hover {
            background-color: var(--accent-blue);
            color: var(--bg-dark);
        }

        .btn-restart {
            background-color: rgba(248, 113, 113, 0.1);
            color: var(--color-danger);
            border-color: var(--color-danger);
        }

        .btn-restart:hover {
            background-color: var(--color-danger);
            color: var(--text-main);
        }

        /* Hilfsklassen */
        .text-success { color: var(--color-success); }
        .text-warning { color: var(--color-warning); }
        .text-cyan { color: var(--accent-blue); }

        /* Responsive Anpassungen */
        @media (max-width: 768px) {
            .bms-layout { flex-direction: column; }
            .sidebar { width: 100%; border-right: none; border-bottom: 1px solid var(--border-color); }
            .cells-grid-2x8 { grid-template-columns: 1fr; }
            .system-actions-row { flex-direction: column; gap: 10px; }
        }
    </style>
</head>

<body onload="show_clock()">

    <div class="bms-layout">
        <!-- Links: Menü / Navigationsleiste -->
        <nav class="sidebar">
            <div class="logo-area">
                <span class="status-indicator online"></span>
                <h1>Status</h1>
            </div>
            <ul class="nav-menu">
                <li><button class="nav-btn active" onclick="switchTab('dashboard', this)">📊 Dashboard</button></li>
                <li><button class="nav-btn" onclick="switchTab('config', this)">⚙️ Konfiguration</button></li>
            </ul>
        </nav>

        <!-- Rechts: Haupt-Inhaltsbereich -->
        <main class="main-content">
            <header class="bms-header">
                <h2 id="page-title">Wattcycle BMS Monitor</h2>
                <div class="system-time"><span><span id="datum"></span> - <span id="uhrzeit"></span></span> | Pack ID: #48V-100Ah</div>
            </header>

            <!-- TAB 1: DASHBOARD -->
            <div id="dashboard" class="tab-content active">
                <div class="bms-grid-top">
                    <!-- Widget 1: SoC -->
                    <section class="card">
                        <h3>Ladezustand (SoC)</h3>
                        <div class="soc-display">
                            <div class="progress-ring">
                                <svg width="140" height="140">
                                    <circle class="ring-bg" cx="70" cy="70" r="60"></circle>
                                    <circle class="ring-fill" cx="70" cy="70" r="60" style="stroke-dashoffset: 56.5;"></circle>
                                </svg>
                                <div class="soc-value">85<span>&percnt;</span></div>
                            </div>
                        </div>
                        <div class="soc-footer">
                            <p>Status: <strong class="text-success">Wird geladen</strong></p>
                        </div>
                    </section>

                    <!-- Widget 2: Hauptmetriken -->
                    <section class="card">
                        <h3>Systemparameter</h3>
                        <div class="metrics-grid">
                            <div class="metric-item">
                                <span class="label">Gesamtspannung</span>
                                <span class="value"><span><span id="VOLTAGES"></span> V</span></span>
                            </div>
                            <div class="metric-item">
                                <span class="label">Stromstärke</span>
                                <span class="value text-success"><span><span id="CURRENT"></span> A</span></span>
                            </div>
                            <div class="metric-item">
                                <span class="label">Leistung</span>
                                <span class="value">1.35 kW</span>
                            </div>
                            <div class="metric-item">
                                <span class="label">Gesundheit (SoH)</span>
                                <span class="value text-cyan">98.2&percnt;</span>
                            </div>
                        </div>
                    </section>

                    <!-- Widget 3: Temperaturen & Alarme -->
                    <section class="card">
                        <h3>Temperatur & Sicherheit</h3>
                        <div class="temp-section">
                            <div class="temp-box">
                                <span class="temp-val">28.4 °C</span>
                                <span class="temp-lbl">Sensor 1 (Zellen)</span>
                            </div>
                            <div class="temp-box">
                                <span class="temp-val">34.1 °C</span>
                                <span class="temp-lbl">Sensor 2 (MOSFET)</span>
                            </div>
                        </div>
                        <div class="alerts-section">
                            <h4>Aktive Alarme</h4>
                            <ul class="alert-list">
                                <li class="alert-item info">Zell-Balancing aktiv</li>
                                <li class="alert-item success">Systemstatus normal</li>
                            </ul>
                        </div>
                    </section>
                </div>

                <!-- Widget 4: Alle 16 Zellen im 2x8 Spalten-Layout ganz unten -->
                <section class="card cells-full-width">
                    <h3>Einzelzellenspannungen</h3>
                    <div class="cells-grid-2x8">
                        <!-- Linke Spalte (Zellen 1-8) -->
                        <div class="cell-column">
                            <div class="cell-row">
                                <span class="cell-name">Zelle 01</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill standard"  id="CELLBAR01" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL01VOLTAGE"></span> V</span>
                            </div>
                            <div class="cell-row">
                                <span class="cell-name">Zelle 02</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill standard" id="CELLBAR02" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL02VOLTAGE"></span> V</span>
                            </div>
                            <div class="cell-row">
                                <span class="cell-name">Zelle 03</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill balancing" id="CELLBAR03" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL03VOLTAGE"></span> V</span>
                            </div>
                            <div class="cell-row">
                                <span class="cell-name">Zelle 04</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill standard" id="CELLBAR04" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL04VOLTAGE"></span> V</span>
                            </div>
                            <div class="cell-row">
                                <span class="cell-name">Zelle 05</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill standard" id="CELLBAR05" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL05VOLTAGE"></span> V</span>
                            </div>
                            <div class="cell-row">
                                <span class="cell-name">Zelle 06</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill standard" id="CELLBAR06" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL06VOLTAGE"></span> V</span>
                            </div>
                            <div class="cell-row">
                                <span class="cell-name">Zelle 07</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill standard" id="CELLBAR07" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL07VOLTAGE"></span> V</span>
                            </div>
                            <div class="cell-row">
                                <span class="cell-name">Zelle 08</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill standard" id="CELLBAR08" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL08VOLTAGE"></span> V</span>
                            </div>
                        </div>

                        <!-- Rechte Spalte (Zellen 9-16) -->
                        <div class="cell-column">
                            <div class="cell-row">
                                <span class="cell-name">Zelle 09</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill standard" id="CELLBAR09" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL09VOLTAGE"></span> V</span>
                            </div>
                            <div class="cell-row">
                                <span class="cell-name">Zelle 10</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill standard" id="CELLBAR10" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL10VOLTAGE"></span> V</span>
                            </div>
                            <div class="cell-row">
                                <span class="cell-name">Zelle 11</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill standard" id="CELLBAR11" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL11VOLTAGE"></span> V</span>
                            </div>
                            <div class="cell-row">
                                <span class="cell-name">Zelle 12</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill standard" id="CELLBAR12" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL12VOLTAGE"></span> V</span>
                            </div>
                            <div class="cell-row">
                                <span class="cell-name">Zelle 13</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill standard" id="CELLBAR13" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL13VOLTAGE"></span> V</span>
                            </div>
                            <div class="cell-row">
                                <span class="cell-name">Zelle 14</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill standard" id="CELLBAR14" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL14VOLTAGE"></span> V</span>
                            </div>
                            <div class="cell-row">
                                <span class="cell-name">Zelle 15</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill danger" id="CELLBAR15" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL15VOLTAGE"></span> V</span>
                            </div>
                            <div class="cell-row">
                                <span class="cell-name">Zelle 16</span>
                                <div class="cell-bar-wrapper">
                                    <div class="cell-bar-fill standard" id="CELLBAR16" style="--cell-mv: 2500;"></div>
                                </div>
                                <span class="cell-volt"><span id="CELL16VOLTAGE"></span> V</span>
                            </div>
                        </div>
                    </div>
                    <div class="cell-summary">
                        <span>Zell-Drift: <strong class="text-warning"><span><span id="CELLDELTA"></span> mV</span></strong></span>
						<span><span>Höchste: <span id="MAXCELLVOLTAGE"></span> V</span> - <span>Niedrigste: <span id="MINCELLVOLTAGE"></span> V</span></span>
                    </div>
                </section>
            </div>

            <!-- TAB 2: KONFIGURATION -->
            <div id="config" class="tab-content">
                <div class="config-layout">
                    <!-- WLAN Konfiguration -->
                    <section class="card">
                        <h3>WLAN Einstellungen</h3>
                        <form onsubmit="event.preventDefault(); alert('WLAN-Einstellungen gespeichert!');">
                            <div class="form-group">
                                <label for="wifi-mode">WLAN Modus</label>
                                <select id="wifi-mode">
                                    <option value="sta">Client (STA-Modus)</option>
                                    <option value="ap">Access Point (AP-Modus)</option>
                                </select>
                            </div>
                            <div class="form-group">
                                <label for="wifi-ssid">SSID (Netzwerkname)</label>
                                <input type="text" id="wifi-ssid" placeholder="Dein_WLAN_Name">
                            </div>
                            <div class="form-group">
                                <label for="wifi-pass">Passwort</label>
                                <input type="password" id="wifi-pass" placeholder="••••••••">
                            </div>
                            <button type="submit" class="btn">WLAN Speichern</button>
                        </form>
                    </section>

                    <!-- MQTT Konfiguration -->
                    <section class="card">
                        <h3>MQTT Broker Einstellungen</h3>
                        <form onsubmit="event.preventDefault(); alert('MQTT-Einstellungen gespeichert!');">
                            <div class="form-group">
                                <label for="mqtt-host">Broker Adresse / IP</label>
                                <input type="text" id="MQTTSERVER" placeholder="192.168.178.50">
                            </div>
                            <div class="form-group">
                                <label for="mqtt-port">Port</label>
                                <input type="number" id="MQTTPORT" value="1883">
                            </div>
                            <div class="form-group">
                                <label for="mqtt-topic">Basis-Topic</label>
                                <input type="text" id="MQTTROOT1" value="wattcycle/bms/">
                            </div>
                            <button type="submit" class="btn">MQTT Speichern</button>
                        </form>
                    </section>

                    <!-- NEUE ZEILE: System-Aktionen (Firmwareupdate & Neustart) -->
                    <div class="system-actions-row">
                        <a href="update"><button class="btn-system btn-update">🔄 Firmwareupdate</button></a>
                        <button class="btn-system btn-restart" onclick="restart();">⚠️ System-Neustart</button>
                    </div>
                </div>
            </div>
        </main>
    </div>


<script>
        // Einfache Tab-Umschaltung
        function switchTab(tabId, element) {
            document.querySelectorAll('.tab-content').forEach(tab => {
                tab.classList.remove('active');
            });
            document.querySelectorAll('.nav-btn').forEach(btn => {
                btn.classList.remove('active');
            });
            
            document.getElementById(tabId).classList.add('active');
            element.classList.add('active');

            const title = document.getElementById('page-title');
            if(tabId === 'dashboard') title.innerText = 'Übersicht Dashboard';
            if(tabId === 'config') title.innerText = 'Systemkonfiguration';
        }

        // System-Aktions-Handler
        function handleUpdate() {
            if(confirm("Möchtest du das Firmware-Update jetzt starten?")) {
                alert("Firmware-Update-Prozess initialisiert...");
            }
        }

        function handleRestart() {
            if(confirm("Möchtest du das BMS-Interface/Hardware jetzt neu starten?")) {
                alert("Neustart-Befehl wird gesendet...");
            }
        }
    </script>


	<script>
    let fetchLaeuft = false;

    async function getDataInterval() {
        if (fetchLaeuft) return;

        fetchLaeuft = true;

        try {
            const response = await fetch('/json', {
                cache: 'no-store'
            });

            if (!response.ok) {
                throw new Error(`HTTP ${response.status}`);
            }

            const data = await response.json();

            const textFelder = [
                'CLIENTID', 'WIFIRSSI', 'MQTTROOT1', 'MQTTSTATECL', 'MQTTSTATE', 'WIFIQUALITI',
                'VOLTAGES', 'CURRENT', 'CELLDELTA', 'CELLMAX',
                'CELLMIN', 'CYCLES'
            ];
			
		
            for (const  id of  textFelder) {
                const element = document.getElementById(id);

                if (element) {
                    element.textContent = data[id] ?? '';
                }
            }
			
			const cellElements = [
				'CELL01VOLTAGE', 'CELL02VOLTAGE', 'CELL03VOLTAGE', 'CELL04VOLTAGE',
				'CELL05VOLTAGE', 'CELL06VOLTAGE', 'CELL07VOLTAGE', 'CELL08VOLTAGE',
                'CELL09VOLTAGE', 'CELL10VOLTAGE', 'CELL11VOLTAGE', 'CELL12VOLTAGE',
                'CELL13VOLTAGE', 'CELL14VOLTAGE', 'CELL15VOLTAGE', 'CELL16VOLTAGE'
			];
			
			for (const  id of  cellElements) {
                const cellelement = document.getElementById(id);

                if (cellelement) {
                    cellelement.textContent = (data[id] / 1000 ?? '').toFixed(3);
                }
            }
			
			
			const cellBarElements = [
				'CELLBAR01', 'CELLBAR02', 'CELLBAR03', 'CELLBAR04', 'CELLBAR05', 'CELLBAR06', 'CELLBAR07', 'CELLBAR08',
                'CELLBAR09', 'CELLBAR10', 'CELLBAR11', 'CELLBAR12', 'CELLBAR13', 'CELLBAR14', 'CELLBAR15', 'CELLBAR16'
			];
			
			let i =1;
			for (const  id of  cellBarElements) {
                const cellbarelement = document.getElementById(id);

                if (cellbarelement) {
					const cellIndex = String(i).padStart(2, '0');
                    cellbarelement.style.setProperty('--cell-mv', data[`CELL${cellIndex}VOLTAGE`]);
                }
				i++;
            }
			
			const maxcellvoltage = document.getElementById('MAXCELLVOLTAGE');
			if (maxcellvoltage){
				maxcellvoltage.textContent = (data.MAXCELLVOLTAGE / 1000 ?? '').toFixed(3);
			}
			
			const mincellvoltage = document.getElementById('MINCELLVOLTAGE');
			if (mincellvoltage){
				mincellvoltage.textContent = (data.MINCELLVOLTAGE / 1000 ?? '').toFixed(3);
			}
			
            const server = document.getElementById('MQTTSERVER');
            const port = document.getElementById('MQTTPORT');
            const mqttAktiv = document.getElementById('CBMQTTSTATE');

            if (server) server.value = data.MQTTSERVER ?? '';
            if (port) port.value = data.MQTTPORT ?? '';

            if (mqttAktiv) {
                mqttAktiv.checked = Boolean(data.CBMQTTSTATE);
            }

        } catch (error) {
            console.error('Fehler beim Abruf der BMS-Daten:', error);
        } finally {
            fetchLaeuft = false;
        }
    }

    getDataInterval();
    setInterval(getDataInterval, 1000);
	</script>


	


	<script>
	  if (!!window.EventSource) {
		  var source = new EventSource('/events');

		  source.addEventListener('open', function(e) {
		    console.log("Events Connected");
		  }, false);

		  source.addEventListener('error', function(e) {
		    if (e.target.readyState != EventSource.OPEN) {
			  console.log("Events Disconnected");
		    }
		  }, false);
				  
	  }


	  function set_power(value) {
		  var xhr = new XMLHttpRequest();
		  xhr.open("GET", '/acoutput?value=' + value, true);
		  xhr.send();
	  };
	  
	  function apmode() {
		  let text = "Reset WiFi credentials and restart!\nPress OK or Cancel.";
		  if (confirm(text) == true) {
		    var xhr = new XMLHttpRequest();
		    xhr.open("GET", "/apmode", true);
		    xhr.send();
		  } else {
		    text = " canceled!";
		  }
	  };

	  function restart() {
		  let text = "Restart System!\nPress OK or Cancel.";
		  if (confirm(text) == true) {
		    var xhr = new XMLHttpRequest();
		    xhr.open("GET", "/restart", true);
		    xhr.send();
		  } else {
		    text = " canceled!";
		  } 
	  };

	  
	  function savesettings() {
		  var mqttserver = document.getElementById("MQTTSERVER").value;
		  var mqttport = document.getElementById("MQTTPORT").value;

		  let text = "Save Settings!\nPress OK or Cancel.";
		  var xhr = new XMLHttpRequest();
		  xhr.open("GET", "/savesettings?mqttserver=" + mqttserver + "&mqttport=" + mqttport, true);
		  xhr.send();    
	  };


	  function toggleCheckbox(element) {
	    var xhr = new XMLHttpRequest();
	    if(element.checked){ xhr.open("GET", "/checkbox?cbid="+element.id+"&state=1", true); }
	    else { xhr.open("GET", "/checkbox?cbid="+element.id+"&state=0", true); }
	    xhr.send();
	  }


	  function show_clock() {
		  var jetzt = new Date();
		  h = jetzt.getHours();
		  m = jetzt.getMinutes();
		  s = jetzt.getSeconds();
		  h = fuehrendeNull(h);
		  m = fuehrendeNull(m);
		  s = fuehrendeNull(s);
		  document.getElementById("uhrzeit").innerHTML = h + ':' + m + ':' + s;
		  setTimeout(show_clock, 1000);
	  };
	  
	  function fuehrendeNull(zahl) {
		  zahl = (zahl < 10 ? '0' : '' ) + zahl;  
		  return zahl;
	  };

	  var mydate = new Date();
	  var options = { year: 'numeric', month: '2-digit', day: '2-digit' };
	  document.getElementById("datum").textContent = mydate.toLocaleString('de-DE', options);

	  
	</script>

	</body>
</html>
)rawliteral";