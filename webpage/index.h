const char* homePagePart1 = R"rawliteral(
  <!doctype html>
  <html lang="en">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover"/>
    <title>Home Care System</title>
    <style>
      body {
        background-color: orange;
        font-family: Arial, sans-serif;
        margin: 0;
        padding: 0;
      }
      .container {
        display: flex;
        flex-direction: column;
        align-items: center;
        padding: 20px;
      }
      h1 {
        font-size: 40px;
        color: black;
        text-align: center;
      }
      p {
        font-size: 25px;
        color: black;
        text-align: center;
      }
      #connectionStatus {
        font-size: 20px;
        text-align: center;
        font-weight: bold;
        color: black;
        margin-top: 10px;
      }
      .EmergencyBox {
        margin: 30px auto;
        padding: 20px;
        border-radius: 10px;
        text-align: center;
        width: 300px;
        background-color: green;
        color: white;
        font-weight: bold;
        box-shadow: rgba(0, 0, 0, 0.17) 0px -23px 25px 0px inset,
                    rgba(0, 0, 0, 0.15) 0px -36px 30px 0px inset,
                    rgba(0, 0, 0, 0.1) 0px -79px 40px 0px inset,
                    rgba(0, 0, 0, 0.06) 0px 2px 1px,
                    rgba(0, 0, 0, 0.09) 0px 4px 2px,
                    rgba(0, 0, 0, 0.09) 0px 8px 4px,
                    rgba(0, 0, 0, 0.09) 0px 16px 8px,
                    rgba(0, 0, 0, 0.09) 0px 32px 16px;
      }

      .DoorBox {
        margin: 30px auto;
        padding: 20px;
        border-radius: 10px;
        text-align: center;
        width: 300px;
        background-color: lightgray;
        color: black;
        font-weight: bold;
        box-shadow: rgba(17, 17, 26, 0.1) 0px 1px 0px,
         rgba(17, 17, 26, 0.1) 0px 8px 24px,
         rgba(17, 17, 26, 0.1) 0px 16px 48px;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <h1>Home Care System</h1>
  
      <!-- Emergency status -->
      <div class="EmergencyBox">
        <div id="NotEmergency" class="status">No Emergency</div>
        <div id="Emergency" class="status">Emergency</div>
      </div>
  
      <!-- Door status -->
      <div class="DoorBox">
        <p>Distance: <span id="UltrasonicValue">Loading...</span> cm</p>
        <p id="Open" style="display:none; color:green;"><b>Door check: Open</b></p>
        <p id="Closed" style="display:none; color:red;"><b>Door check: Closed</b></p>
      </div>
      
      <!-- Debug -->
      <div id="connectionStatus">Connecting...</div>
      <div id="lastResponse" style="display:none"></div>
    </div>
  
    <script>
      // --- Emergency Box logic ---
      function CallButtonStatus(callbutton) {
        const EmerBox = document.querySelector(".EmergencyBox");
        const Emer = document.getElementById("Emergency");
        const NEmer = document.getElementById("NotEmergency");
  
        if (callbutton) {
          NEmer.style.display = "none";
          Emer.style.display = "block";
          EmerBox.style.background = "red";
        } else {
          Emer.style.display = "none";
          NEmer.style.display = "block";
          EmerBox.style.background = "green";
        }
      }
  
      function fetchCallButton() {
        fetch("/Callbutton")
          .then((response) => {
            document.getElementById("connectionStatus").textContent = "Devices Connected";
            return response.text();
          })
          .then((responseText) => {
            document.getElementById("lastResponse").textContent = responseText;
            const callbutton = responseText.trim().toLowerCase() === "true" || responseText.trim() === "1";
            CallButtonStatus(callbutton);
          })
          .catch((error) => {
            console.error("Error fetching call button:", error);
            document.getElementById("connectionStatus").textContent = "Error: " + error.message;
          });
      }
  
      // --- Ultrasonic Door logic ---
      function fetchUltrasonic() {
        fetch("/Ultrasonic")
          .then(response => response.text())
          .then(sonic => {
            console.log("Ultrasonic:", sonic);
            document.getElementById("UltrasonicValue").innerText = sonic;
            doorStatus(parseFloat(sonic));
          })
          .catch(error => console.error('Error fetching Ultrasonic:', error));
      }

      function doorStatus(distance) {
        const openEl = document.getElementById("Open");
        const closedEl = document.getElementById("Closed");
  
        if (distance < 15) {
          closedEl.style.display = "none";
          openEl.style.display = "block";
        } else {
          openEl.style.display = "none";
          closedEl.style.display = "block";
        }
      }
  
      // --- On page load ---
      window.onload = () => {
        document.getElementById("NotEmergency").style.display = "none";
        document.getElementById("Emergency").style.display = "none";
  
        fetchCallButton();
        fetchUltrasonic();
  
        setInterval(fetchCallButton, 1000);
        setInterval(fetchUltrasonic, 1000);
      };
    </script>
  </body>
  </html>
  )rawliteral";
