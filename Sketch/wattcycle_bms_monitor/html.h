const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
	<head>
		<title>WattCycle BMS Monitor</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">

		<style>
			html {
        font-family: Arial;
        display: inline-block;
        text-align: center;
      }
			
      table { 
        width:90%;
        table-layout: fixed;
      }
			
      table.font1{
        font-size:clamp(.9rem, 0.9vw, 1.2rem);
      }
			
      table.style2 {
        width: 100%;
        table-layout: fixed;
      }
			
      td {
        text-align: left;
      }
			
      th {
        text-align: center;
      }

			p {
        font-size: 1.2rem;
      }

      body {
        margin: 0;
      }

			.title1 {
        font-size: 1.2rem;
        font-weight: bold;
        color: #034078;
      }
			.alnright { text-align: right; }
			.topnav { overflow: hidden; background-color: #263c5e; color: white; font-size:1.5rem; margin:0px; padding:0px; }
			.content { padding: 10px; }
			.card { background-color: white; padding: 5px; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
			.card-grid { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(20em, 1fr)); }
			.reading { font-size: 2.0rem; }
			.headline { font-size: 2.0rem; font-weight: bold; }
			.card.info { color: #034078; }
			.card.info-dark { color: #151515; }
			.card-title { font-size: 1.2rem; font-weight: bold; color: #034078; }
			.card-title-dark { font-size: 1.2rem; font-weight: bold; color: #151515; }
			.btn:active { transform: scale(0.80); box-shadow: 3px 2px 22px 1px rgba(0, 0, 0, 0.24); }
			.btn {
					text-decoration: none;
					border: none;
					width: 90px;
					padding: 10px 10px;
					margin: 15px 5px 15px 5px;
					text-align: center;
					font-size: 16px;
					background-color:#034078;
					color: #fff;
					border-radius: 5px;
					box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24);
					cursor: pointer;
					outline: none;
					transition: 0.2s all;
				}
			
			.flexBox1 {
			  display: flex;
			  flex-direction: row;
			  justify-content: space-between;
			  align-items: center;
			  text-align: center;
			}

			.flexBox2 {
			  display: flex;
			  flex-direction: row;
			  justify-content: space-around;
			  align-items: center;
			  text-align: center;
			}

			input:invalid {
				background-color: rgb(255, 179, 190);
			}

			input[type=text], select {
				padding: 5px 5px;
				margin: 8px 0;
				display: inline-block;
				border: 1px solid #ccc;
				border-radius: 4px;
				box-sizing: border-box;
			}

			input[type=number], select {
				padding: 5px 5px;
				margin: 8px 0;
				display: inline-block;
				border: 1px solid #ccc;
				border-radius: 4px;
				box-sizing: border-box;
			}

			input[type=time], select {
				padding: 5px 5px;
				margin: 8px 0;
				display: inline-block;
				border: 1px solid #ccc;
				border-radius: 4px;
				box-sizing: border-box;
			}     
			 
			hr {
				border: 0;
				height: 1px;
				background-image: linear-gradient(to right, rgba(0, 0, 0, 0), rgba(0, 0, 0, 0.75), rgba(0, 0, 0, 0));
			}
		   
			summary {
				position: relative;
				padding: 12px;
				display: flex;
				color: #102752;
				transform: scaleX(-1);
			}

			summary b {
				display: inline-block;
				position: absolute;
				right: 6px;
				transform: scaleX(-1);
			}

			details>summary {
				list-style: none;
			}

			summary::-webkit-details-marker {
				display: none;
			}

			summary::after {
				content: '+';
				color: #999999;  
			}
			
			details[open] summary:after {
				  content: "-";
			  color: #999999; 
			}

			.detailsFlexBox{
			  display: flex;
			  flex-direction: row;
			  justify-content:flex-start;
			  align-items: center;
			  gap: 10px;
			  margin-left: 10px;
			  margin-top: 5px;
			  margin-bottom: 5px;
			}

			.flexContainer{
			  display: flex;
			  flex-direction: row;
			  justify-content:flex-start;
			  align-items: center;
			  margin-left: 10px;
			  margin-top: 5px;
			}

			.flexColLeft{
			  flex-basis: 65%;
			  text-align: start;
			  padding-left: 5px;
			}
		  
			.flexColRight{
			  flex-basis: 35%;
			  text-align: end;
			  padding-right: 25px;
			}

			.fontBold{
			  font-weight: bold;
			  text-align: left;
			  margin-left: 10px;
			}

			.fontNormal{
			  font-weight: normal;
			  text-align: left;
			  margin-left: 10px;
			}

			.cellStyle1{
			  width: 140px;
			  text-align: left;
			}

			.cellStyle2{
			  width: 80px;
			  text-align: left;
			}

			.cellStyle3{
				width: 200px;
				text-align: left;
			}

		   
			.version {
				top: -40%;
				right: -165px;
				font-size: 40%;
				margin-top: -25px;
				margin-bottom: 10px;
				position: relative;
			}
			
			
			.container {
				background-color: #fff;
				padding: 30px;
				border-radius: 8px;
				box-shadow: 0 4px 6px rgba(0,0,0,0.1);
				max-width: 700px;
				width: 400px;
			}
			
			h2 {
				  text-align: center;
				  color: #2c3e50;
				  margin-top: 0;
				  margin-bottom: 20px;
			}
			
			.table-container {
			  display: flex;
        gap: 10px;
        width: 100%;
        min-width: 0;
        box-sizing: border-box;
        align-items: flex-start;
			}
			
			table.bat {
				width: 0;
        flex: 1 1 0;
        min-width: 0;
        box-sizing: border-box;
        border-collapse: collapse;
        table-layout: fixed;
				margin-bottom: 20px;
			}
			
			table.bat th {
				border: 1px solid #ddd;
				padding: 12px;
				text-align: center;
			}
			
			table.bat td {
				border: 1px solid #ddd;
				padding: 12px;
				text-align: center;
			}
			
			table.bat th {
				background-color: #263c5e;
				color: white;
				font-weight: bold;
			}
			
			table.bat tr:nth-child(even) {
				background-color: #f9f9f9;
			}
				  
			.voltage {
				font-weight: bold;
				color: #2e7d32;
      }
			
		</style>
	</head>
	<body onload="show_clock()">

	<div class="topnav">
		<h3>WattCycle BMS Monitor</h3>
	</div>

	<div class="content">
		<div class="card-grid">
			<div class="card">
				<details>
					<summary>
						<b>
							<table>
								<tbody>
								<tr>
									<td width="150px">ESP</td>
									<td class="alnright"><span>WiFi-Qualit&aumlt: <span id="WIFIQUALITI"></span> &percnt;</span></td>
								</tr>
								</tbody>
							</table>
						</b>
					</summary>
					<hr>
					<div class="detailsFlexBox">
						<div class="cellStyle1">Client ID:</div>
						<div id="CLIENTID"></div>
					</div>
					<div class="detailsFlexBox">
						<div class="cellStyle1">Wifi RSSI:</div>
						<div><span><span id="WIFIRSSI"></span> dBm</span></div>
					</div>
				</details>
			</div>
		</div>
	</div>

	  
	<div class="content">
		<div class="card-grid">
			<div class="card">
				<details>
					<summary>
						<b>
							<table>
								<tbody>
									<tr>
										<td width="180px"><span>MQTT</span></td>
										<td class="alnright"><span id="MQTTSTATE"></span></td>
									</tr>
								</tbody>
							</table>
						</b>
					</summary>
					<hr>
					<div class="flexContainer">
						<div class="flexColLeft">MQTT Server:</div>
						<div class="flexColRight"><input type='text' id='MQTTSERVER' placeholder='xxx.xxx.xxx.xxx' required pattern='^((\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.){3}(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])$'/></div>
					</div>
					<div class="flexContainer">
						<div class="flexColLeft">MQTT Port:</div>
						<div class="flexColRight"><input type="number" id="MQTTPORT" min="0" max="65535" /></div>
					</div>
					<div class="flexContainer">
						<div style="padding-right: 50px;">Aktiv: <input type="checkbox" onchange="toggleCheckbox(this)" id="CBMQTTSTATE" /></div>
						<div><span>Status: </span><span id="MQTTSTATECL"></span></div>
					</div>
					<hr>
					<div class="fontBold">Publish Topics</div>
					<div class="flexContainer">
						<div class="flexColLeft"><span><span id="MQTTROOT1"></span></span></div>
					</div>
					<br>
				</details>
			</div>
		</div>
	</div>
	  
	  
		  
	<div class="content">
		<div class="card-grid">
			<div class="card">
				<h2>BMS-Daten</h2>
		  
				<table class="bat" style="width:100%">
					<tr>
						<td>
							<div class="detailsFlexBox">
								<div class="cellStyle1">Spannung:</div>
								<div> <span><span id="VOLTAGES"></span> V</span></div>
							</div>
						</td>
				
						<td>
							<div class="detailsFlexBox">
								<div class="cellStyle3">Max Zellenspannung: <span>C<span id="CELLMAX"></span></span></div>
								<div> <span><span id="MAXCELLVOLTAGE"></span> mV</span></div>
							</div>
						</td>
					</tr>
				
					<tr>
						<td>
							<div class="detailsFlexBox">
								<div class="cellStyle1">Strom:</div>
								<div> <span><span id="CURRENT"></span> A</span></div>
							</div>
						</td>
				
						<td>
							<div class="detailsFlexBox">
								<div class="cellStyle3">Min Zellenspannung: <span>C<span id="CELLMIN"></span></span></div>
								<div> <span><span id="MINCELLVOLTAGE"></span> mV</span></div>
							</div>
						</td>
					</tr>
			  
					<tr>
						<td>
							<div class="detailsFlexBox">
								<div class="cellStyle1">Ladezyklen:</div>
								<div> <span><span id="CYCLES"></span></span></div>
							</div>
						</td>
				
						<td>
							<div class="detailsFlexBox">
								<div class="cellStyle3">Zellendifferenz:</div>
								<div> <span><span id="CELLDELTA"></span> mV</span></div>
							</div>
						</td>
					</tr>						  
				</table>

					  
				<div class="table-container">
					<table class="bat">
						<thead>
							<tr>
								<th>Zelle</th>
								<th>Spannung (mV)</th>
							</tr>
						</thead>
						<tbody>
							<tr><td>Zelle 1</td><td class="voltage"><span id="CELL01VOLTAGE"></span></td></tr>
							<tr><td>Zelle 2</td><td class="voltage"><span id="CELL02VOLTAGE"></span></td></tr>
							<tr><td>Zelle 3</td><td class="voltage"><span id="CELL03VOLTAGE"></span></td></tr>
							<tr><td>Zelle 4</td><td class="voltage"><span id="CELL04VOLTAGE"></span></td></tr>
							<tr><td>Zelle 5</td><td class="voltage"><span id="CELL05VOLTAGE"></span></td></tr>
							<tr><td>Zelle 6</td><td class="voltage"><span id="CELL06VOLTAGE"></span></td></tr>
							<tr><td>Zelle 7</td><td class="voltage"><span id="CELL07VOLTAGE"></span></td></tr>
							<tr><td>Zelle 8</td><td class="voltage"><span id="CELL08VOLTAGE"></span></td></tr>
						</tbody>
					</table>

					<table class="bat">
						<thead>
							<tr>
								<th>Zelle</th>
								<th>Spannung (mV)</th>
							</tr>
						</thead>
						<tbody>
							<tr><td>Zelle 9</td><td class="voltage"><span id="CELL09VOLTAGE"></span></td></tr>
							<tr><td>Zelle 10</td><td class="voltage"><span id="CELL10VOLTAGE"></span></td></tr>
							<tr><td>Zelle 11</td><td class="voltage"><span id="CELL11VOLTAGE"></span></td></tr>
							<tr><td>Zelle 12</td><td class="voltage"><span id="CELL12VOLTAGE"></span></td></tr>
							<tr><td>Zelle 13</td><td class="voltage"><span id="CELL13VOLTAGE"></span></td></tr>
							<tr><td>Zelle 14</td><td class="voltage"><span id="CELL14VOLTAGE"></span></td></tr>
							<tr><td>Zelle 15</td><td class="voltage"><span id="CELL15VOLTAGE"></span></td></tr>
							<tr><td>Zelle 16</td><td class="voltage"><span id="CELL16VOLTAGE"></span></td></tr>
						</tbody>
					</table>
				</div>
		
			</div>
		</div>
	</div>


	<div class="content">
		<div class="card-grid">
			<div class="card">
				<div class="flexBox1">
					<button class="btn" type="button" onclick="apmode();" style=" font-size: 10px; width: auto; margin: 5px 5px 5px 5px; background-color:#767676;">Delete WiFi Settings</button>
					<button class="btn" type="button" onclick="restart();" style=" font-size: 10px; width: auto; margin: 5px 5px 5px 5px; background-color:#767676;">Restart</button>
					<a href="update"><button class="btn" type="button" style="font-size: 10px; width: auto; margin: 5px 5px 5px 5px; background-color:#767676;"> FW Update</button></a>
					<button class="btn" type="button" onclick="savesettings();" style=" font-size: 10px; width: auto; margin: 5px 5px 5px 5px; background-color:#767676;">Save Settings</button>
				</div>
			</div>
		</div>
	</div>

	<div>
		<span><span id="datum"></span> - <span id="uhrzeit"></span></span>
	</div>


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
                'VOLTAGES', 'CURRENT', 'MAXCELLVOLTAGE', 'MINCELLVOLTAGE', 'CELLDELTA', 'CELLMAX',
                'CELLMIN', 'CYCLES',
                'CELL01VOLTAGE', 'CELL02VOLTAGE', 'CELL03VOLTAGE', 'CELL04VOLTAGE',
                'CELL05VOLTAGE', 'CELL06VOLTAGE', 'CELL07VOLTAGE', 'CELL08VOLTAGE',
                'CELL09VOLTAGE', 'CELL10VOLTAGE', 'CELL11VOLTAGE', 'CELL12VOLTAGE',
                'CELL13VOLTAGE', 'CELL14VOLTAGE', 'CELL15VOLTAGE', 'CELL16VOLTAGE'
            ];

            for (const id of textFelder) {
                const element = document.getElementById(id);

                if (element) {
                    element.textContent = data[id] ?? '';
                }
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