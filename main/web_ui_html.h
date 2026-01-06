/* Armageddon Framework v3 - web_ui_html.h
 * Interface web dark mode cyberpunk - HTML/CSS/JS inline
 * Vert fluo #00ff41, animations, glow effects
 */

#ifndef WEB_UI_HTML_H
#define WEB_UI_HTML_H

const char INDEX_HTML[] = R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ARMAGEDDON v3</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            background: linear-gradient(135deg, #0a0e27 0%, #16213e 100%);
            color: #00ff41;
            font-family: 'Courier New', monospace;
            overflow: hidden;
        }
        
        #container {
            display: flex;
            flex-direction: column;
            height: 100vh;
        }
        
        header {
            background: rgba(0, 0, 0, 0.8);
            padding: 20px;
            border-bottom: 2px solid #00ff41;
            text-align: center;
            position: relative;
        }
        
        h1 {
            font-size: 2.5em;
            letter-spacing: 3px;
            animation: glow 2s ease-in-out infinite;
            text-shadow: 0 0 10px #00ff41, 0 0 20px #00ff41;
        }
        
        @keyframes glow {
            0%, 100% { text-shadow: 0 0 10px #00ff41, 0 0 20px #00ff41; }
            50% { text-shadow: 0 0 20px #00ff41, 0 0 40px #00ff41, 0 0 60px #00ff41; }
        }
        
        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.7; }
        }
        
        .tabs {
            display: flex;
            background: rgba(0, 0, 0, 0.6);
            border-bottom: 1px solid #00ff41;
            gap: 0;
        }
        
        .tab-button {
            flex: 1;
            padding: 15px;
            background: transparent;
            color: #00ff41;
            border: 1px solid #00ff41;
            border-bottom: none;
            cursor: pointer;
            font-family: 'Courier New', monospace;
            font-size: 1em;
            font-weight: bold;
            transition: all 0.3s;
        }
        
        .tab-button:hover {
            background: rgba(0, 255, 65, 0.1);
            box-shadow: 0 0 10px #00ff41;
        }
        
        .tab-button.active {
            background: rgba(0, 255, 65, 0.2);
            box-shadow: inset 0 0 10px #00ff41;
        }
        
        #content {
            flex: 1;
            overflow-y: auto;
            padding: 20px;
            background: rgba(10, 14, 39, 0.9);
        }
        
        .tab-content {
            display: none;
        }
        
        .tab-content.active {
            display: block;
            animation: fadeIn 0.3s;
        }
        
        @keyframes fadeIn {
            from { opacity: 0; }
            to { opacity: 1; }
        }
        
        textarea {
            width: 100%;
            height: 300px;
            background: #0a0e27;
            color: #00ff41;
            border: 1px solid #00ff41;
            padding: 10px;
            font-family: 'Courier New', monospace;
            font-size: 0.9em;
            resize: vertical;
        }
        
        textarea:focus {
            outline: none;
            box-shadow: 0 0 15px #00ff41;
        }
        
        .control-group {
            margin: 15px 0;
            display: flex;
            gap: 10px;
            align-items: center;
        }
        
        label {
            min-width: 120px;
            font-weight: bold;
        }
        
        select, input[type="number"] {
            background: #0a0e27;
            color: #00ff41;
            border: 1px solid #00ff41;
            padding: 8px;
            font-family: 'Courier New', monospace;
            flex: 1;
        }
        
        select:focus, input:focus {
            outline: none;
            box-shadow: 0 0 10px #00ff41;
        }
        
        button {
            background: #00ff41;
            color: #0a0e27;
            border: none;
            padding: 12px 30px;
            font-weight: bold;
            cursor: pointer;
            font-family: 'Courier New', monospace;
            font-size: 1em;
            transition: all 0.3s;
            box-shadow: 0 0 10px rgba(0, 255, 65, 0.5);
        }
        
        button:hover {
            box-shadow: 0 0 20px #00ff41;
            transform: scale(1.05);
        }
        
        button:active {
            transform: scale(0.95);
        }
        
        button.danger {
            background: #ff4141;
            box-shadow: 0 0 10px rgba(255, 65, 65, 0.5);
        }
        
        button.danger:hover {
            box-shadow: 0 0 20px #ff4141;
        }
        
        .network-list {
            max-height: 300px;
            overflow-y: auto;
            border: 1px solid #00ff41;
            background: rgba(0, 255, 65, 0.05);
        }
        
        .network-item {
            padding: 10px;
            border-bottom: 1px solid #00ff41;
            cursor: pointer;
            transition: all 0.2s;
        }
        
        .network-item:hover {
            background: rgba(0, 255, 65, 0.1);
        }
        
        .network-item.selected {
            background: rgba(0, 255, 65, 0.2);
            font-weight: bold;
        }
        
        #console {
            width: 100%;
            height: 400px;
            background: #0a0e27;
            border: 1px solid #00ff41;
            padding: 10px;
            font-family: 'Courier New', monospace;
            font-size: 0.85em;
            overflow-y: auto;
            color: #00ff41;
        }
        
        .status {
            margin-top: 15px;
            padding: 10px;
            border: 1px solid #00ff41;
            border-radius: 3px;
            background: rgba(0, 255, 65, 0.05);
        }
        
        .status.error {
            border-color: #ff4141;
            color: #ff4141;
        }
        
        .status.success {
            border-color: #00ff41;
            color: #00ff41;
        }
        
        .status.info {
            border-color: #00ffff;
            color: #00ffff;
        }
        
        .progress-bar {
            width: 100%;
            height: 20px;
            background: rgba(0, 255, 65, 0.1);
            border: 1px solid #00ff41;
            overflow: hidden;
            margin: 10px 0;
        }
        
        .progress-fill {
            height: 100%;
            background: linear-gradient(90deg, #00ff41, #00ffff);
            width: 0%;
            transition: width 0.1s;
            box-shadow: 0 0 10px #00ff41;
        }
    </style>
</head>
<body>
    <div id="container">
        <header>
            <h1>[SKULL] ARMAGEDDON v3 [SKULL]</h1>
            <small>ESP32-S3 Pentest Framework</small>
        </header>
        
        <div class="tabs">
            <button class="tab-button active" onclick="switchTab('badusb')">BadUSB [KEYS]</button>
            <button class="tab-button" onclick="switchTab('wifi')">WiFi [WIFI]</button>
            <button class="tab-button" onclick="switchTab('logs')">Logs [LOGS]</button>
        </div>
        
        <div id="content">
            <!-- BadUSB Tab -->
            <div id="badusb" class="tab-content active">
                <h2>DuckyScript Executor</h2>
                <label>Keyboard Layout:</label>
                <select id="layout">
                    <option value="0">US QWERTY</option>
                    <option value="1">FR AZERTY (Accents)</option>
                    <option value="2">DE QWERTZ</option>
                </select>
                
                <label>DuckyScript Payload:</label>
                <textarea id="duckyscript" placeholder="STRING Hello World&#10;DELAY 500&#10;ENTER"></textarea>
                
                <div class="control-group">
                    <button onclick="executeDucky()" style="flex: 1;">[>] EXECUTE PAYLOAD</button>
                </div>
                <div id="badusb-status" class="status" style="display:none;"></div>
            </div>
            
            <!-- WiFi Tab -->
            <div id="wifi" class="tab-content">
                <h2>WiFi Attack Suite</h2>
                
                <button onclick="scanNetworks()">[SEARCH] SCAN NETWORKS</button>
                <div id="scan-status" class="status" style="display:none;"></div>
                
                <label>Available Networks:</label>
                <div id="network-list" class="network-list"></div>
                
                <div id="selected-network" style="margin-top: 15px; padding: 10px; border: 1px solid #00ff41; display:none;">
                    <strong>Selected: <span id="target-ssid"></span></strong>
                </div>
                
                <div class="control-group">
                    <button id="deauth-btn" class="danger" onclick="startDeauth()" disabled style="flex: 1;">
                        [!] LAUNCH DEAUTH ATTACK
                    </button>
                </div>
                
                <div id="deauth-status" class="status" style="display:none;"></div>
                <div id="attack-progress" class="progress-bar" style="display:none;">
                    <div class="progress-fill"></div>
                </div>
            </div>
            
            <!-- Logs Tab -->
            <div id="logs" class="tab-content">
                <h2>System Console</h2>
                <button onclick="clearLogs()">🗑 CLEAR LOGS</button>
                <button onclick="updateLogs()" style="margin-left: 10px;">🔄 REFRESH</button>
                <div id="console"></div>
            </div>
        </div>
    </div>
    
    <script>
        let selectedNetworkIdx = -1;
        
        function switchTab(tab) {
            document.querySelectorAll('.tab-content').forEach(e => e.classList.remove('active'));
            document.querySelectorAll('.tab-button').forEach(e => e.classList.remove('active'));
            document.getElementById(tab).classList.add('active');
            event.target.classList.add('active');
        }
        
        function showStatus(elementId, msg, type = 'info') {
            const el = document.getElementById(elementId);
            el.textContent = msg;
            el.className = 'status ' + type;
            el.style.display = 'block';
        }
        
        async function scanNetworks() {
            showStatus('scan-status', '[SEARCH] Scanning...', 'info');
            try {
                const res = await fetch('/scan');
                const data = await res.json();
                
                const list = document.getElementById('network-list');
                list.innerHTML = '';
                
                data.networks.forEach((net, idx) => {
                    const item = document.createElement('div');
                    item.className = 'network-item';
                    item.innerHTML = `${net.ssid} | CH:${net.channel} | RSSI:${net.rssi} ${net.secure ? '🔒' : '🔓'}`;
                    item.onclick = () => selectNetwork(idx, net.ssid, net.bssid);
                    list.appendChild(item);
                });
                
                showStatus('scan-status', `[OK] Found ${data.networks.length} networks`, 'success');
            } catch (e) {
                showStatus('scan-status', '[FAIL] Scan failed: ' + e, 'error');
            }
        }
        
        function selectNetwork(idx, ssid, bssid) {
            selectedNetworkIdx = idx;
            document.getElementById('target-ssid').textContent = ssid;
            document.getElementById('selected-network').style.display = 'block';
            document.getElementById('deauth-btn').disabled = false;
            document.querySelectorAll('.network-item').forEach((e, i) => {
                e.classList.toggle('selected', i === idx);
            });
        }
        
        async function startDeauth() {
            if (selectedNetworkIdx < 0) return;
            
            showStatus('deauth-status', '[!] Attack in progress...', 'info');
            document.getElementById('attack-progress').style.display = 'block';
            
            try {
                const res = await fetch('/deauth', {
                    method: 'POST',
                    headers: {'Content-Type': 'application/json'},
                    body: JSON.stringify({idx: selectedNetworkIdx})
                });
                const data = await res.json();
                
                if (data.success) {
                    showStatus('deauth-status', `[OK] Attack complete! ${data.packets} packets sent`, 'success');
                } else {
                    showStatus('deauth-status', '[FAIL] Attack failed: ' + data.error, 'error');
                }
            } catch (e) {
                showStatus('deauth-status', '[FAIL] Error: ' + e, 'error');
            }
            document.getElementById('attack-progress').style.display = 'none';
        }
        
        async function executeDucky() {
            const script = document.getElementById('duckyscript').value;
            const layout = document.getElementById('layout').value;
            
            showStatus('badusb-status', '⌨ Executing...', 'info');
            
            try {
                const res = await fetch('/badusb', {
                    method: 'POST',
                    headers: {'Content-Type': 'application/json'},
                    body: JSON.stringify({script, layout: parseInt(layout)})
                });
                const data = await res.json();
                
                if (data.success) {
                    showStatus('badusb-status', `[OK] Payload executed!`, 'success');
                } else {
                    showStatus('badusb-status', '[FAIL] Failed: ' + data.error, 'error');
                }
            } catch (e) {
                showStatus('badusb-status', '[FAIL] Error: ' + e, 'error');
            }
        }
        
        async function updateLogs() {
            try {
                const res = await fetch('/logs');
                const data = await res.json();
                const console = document.getElementById('console');
                console.innerHTML = data.logs.replace(/</g, '&lt;').replace(/>/g, '&gt;');
                console.scrollTop = console.scrollHeight;
            } catch (e) {
                console.log('Log update failed:', e);
            }
        }
        
        function clearLogs() {
            document.getElementById('console').innerHTML = '';
        }
        
        setInterval(updateLogs, 1000);
    </script>
</body>
</html>
)HTML";

#endif /* WEB_UI_HTML_H */
