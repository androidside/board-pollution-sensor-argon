

async function getTemperature() {
    const result = await fetch("/api/temperature");
    const temperature = await result.json();
    console.log(temperature);
    const el = document.getElementById("temperature-val");
    el.innerText = temperature.temperature;
  }
  setInterval(getTemperature, 30000);
  
async function getDatabase() {
    const result = await fetch("/api/database");
    const database_json = await result;
    console.log(database_json);
    const el = document.getElementById("database-val");
    el.innerText = database_json.database;
  }
  setInterval(getDatabase, 1000);

  let isLedOn = false;
  async function toggleLed() {
    const el = document.getElementById("led-button");
    isLedOn = !isLedOn;
    fetch("api/led", { method: "POST", body: JSON.stringify({ isLedOn }) });
    if (isLedOn) {
      el.classList.add("led-on");
      el.classList.remove("led-off");
    } else {
      el.classList.add("led-off");
      el.classList.remove("led-on");
    }
  }


// async function getTemperature() {
//     const result = await fetch("/api/temperature"); //we create a local route and the -P will fetch from the ESP32 address when it doesnt find it locally
//     const temperature = await result.json();
//     console.log(temperature);
//     const el = document.getElementById("temperature-val"); //From span id on index.htmml
//     el.innerText = temperature.temperature; //temperature is the object we get in, and temperature is the value
// }
// //We create a timer with a second
// setInterval(getTemperature, 1000); //First argument = temperature that is going to be called
// //Second argument =  Duration in miliseconds. This should call getTemperature every second



// // Create a global variable that is gonna hold the state of the LED
//   let isLedOn = false;
//   //Implement the method defined on the index.html div id="led-button"
//   async function toggleLed() {
//     const el = document.getElementById("led-button");
//     isLedOn = !isLedOn; //change status of the LED variable
//     fetch("api/led", { method: "POST", body: JSON.stringify({ isLedOn }) }); //The body is going to be the string of a JSON object
//     //where the key is going to isLedOn, and the value is going to be ...
//     if(isLedOn){
//     //we are going to set the "led-on" class and remove the "led-off" class
//       el.classList.add("led-on");
//       el.classList.remove("led-off");
//     } else {
//       el.classList.add("led-off");
//       el.classList.remove("led-on");
//     }
//   }