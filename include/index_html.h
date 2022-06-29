const char INDEX_HTML[] PROGMEM = R"=====(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Flapping Web Controller</title>
  <style>
    html {font-family: Times New Roman; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 1.9rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #38c0ff  ;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background:#01070a; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #01070a; cursor: pointer; } 
  </style>
</head>
<body>
  <h2>Flapping Web Controller</h2>
  <p><span id="textslider_value">%SLIDERVALUE%</span></p>
  <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="255" value="%SLIDERVALUE%" step="1" class="slider"></p>
  <div id="zone_joystick" style="position: relative; top:100px; left:50%; "></div>
  
  <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/nipplejs/0.7.3/nipplejs.js"></script>
  <script type="text/javascript" type="text/javascript">
    function updateSliderPWM(element) {
      var slider_value = document.getElementById("pwmSlider").value;
      document.getElementById("textslider_value").innerHTML = slider_value;
      console.log(slider_value);
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/slider?value="+slider_value, true);
      xhr.send();
    }

    createJoystick = function () {
      manager = nipplejs.create();
    }

    const inputElem = document.getElementById('pwmSlider'); // input要素
    const currentValueElem = document.getElementById('textslider_value'); // 埋め込む先のspan要素

    // 現在の値をspanに埋め込む関数
    const setCurrentValue = (val) => {
      currentValueElem.innerText = val;
    }

    // inputイベント時に値をセットする関数
    const rangeOnChange = (e) =>{
      setCurrentValue(e.target.value);
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/slider?value="+e.target.value, true);
      xhr.send();
    }

    window.onload = () => {
      createJoystick();
      inputElem.addEventListener('input', rangeOnChange); // スライダー変化時にイベントを発火
      setCurrentValue(inputElem.value); // ページ読み込み時に値をセット
    }
  </script>
</body>

</html>
)=====";