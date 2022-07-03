const char INDEX_HTML[] PROGMEM = R"=====(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Flapping Web Controller</title>
  <style>
    html {font-family: Times New Roman; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 1.9rem;}
    body {max-width: 360px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: slider-vertical; margin: 14px; width: 180px; height: 360px; background: #93cf32  ;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background:#01070a; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #01070a; cursor: pointer;} 
    .wrap{
      display: -webkit-flex;
      display: flex;
      -webkit-justify-content: center;
      justify-content: center;
      -webkit-align-items: center;
      align-items: center;
      height: 500px;
    }    
    .set{
      display: -webkit-flex;
      display: flex;
      -webkit-align-items: flex-start;
      align-items: flex-start;
      -webkit-flex-direction: column;
      flex-direction: column;
      -webkit-justify-content: space-around;
      justify-content: space-between;
      position: relative;
    }
    .slider-val{
      background: yellow;
      position: absolute;
      top: 0%;
      left: 0%;
      transform: translateY(0%) translateX(-50%);
    }    
  </style>
</head>
<body>
  <h2>Flapping Web Controller</h2>
  <div class="wrap">
    <div class="set">
     <p><input type="range" onchange="updateSliderPWM(this, 'pwmSlider', 'textslider_value')" id="pwmSlider" min="0" max="255" value="%SLIDERVALUE%" step="1" class="slider" orient="vertical"</p>
     <p><span id="textslider_value" class="slider-val">%SLIDERVALUE%</span></p>
    </div>
    
    <div class="set">
      <p><input type="range" onchange="updateSliderPWM(this, 'pwmSlider2', 'textslider_value2')" id="pwmSlider2" min="0" max="255" value="%SLIDERVALUE%" step="1" class="slider" orient="vertical"</p>
      <p><span id="textslider_value2" class="slider-val">%SLIDERVALUE%</span></p>
    </div>
  </div>

  
<script>
function updateSliderPWM(element, elem_id, id_text) {
  document.getElementById(elem_id).value = 0;
  document.getElementById(id_text).innerHTML = 0;
  var slider_value = document.getElementById(elem_id).value;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+slider_value, true);
  xhr.send();
}
</script>

<br><br>
<script>
const inputElem = document.getElementById('pwmSlider'); // input要素
const inputElem2 = document.getElementById('pwmSlider2'); // input要素
const currentValueElem = document.getElementById('textslider_value'); // 埋め込む先のspan要素
const currentValueElem2 = document.getElementById('textslider_value2'); // 埋め込む先のspan要素

// inputイベント時に値をセットする関数
const rangeOnChange = (e) =>{
  currentValueElem.innerText = e.target.value;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+e.target.value, true);
  xhr.send();
}
const rangeOnChange2 = (e) =>{
  currentValueElem2.innerText = e.target.value;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+e.target.value, true);
  xhr.send();
}


window.onload = () => {
  inputElem.addEventListener('input', rangeOnChange); // スライダー変化時にイベントを発火
  inputElem2.addEventListener('input', rangeOnChange2); // スライダー変化時にイベントを発火
  currentValueElem.innerText = inputElem.value;
  currentValueElem2.innerText = inputElem2.value;
}
</script>

</body>
</html>
)=====";