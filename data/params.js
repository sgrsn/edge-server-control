document.addEventListener('DOMContentLoaded', function() {
    const param1Slider = document.getElementById('param1');
    const param2Slider = document.getElementById('param2');
    const param3Slider = document.getElementById('param3');
    const param4Slider = document.getElementById('param4');
    const param5Slider = document.getElementById('param5');
    const param6Slider = document.getElementById('param6');
    const param1Value = document.getElementById('param1-value');
    const param2Value = document.getElementById('param2-value');
    const param3Value = document.getElementById('param3-value');
    const param4Value = document.getElementById('param4-value');
    const param5Value = document.getElementById('param5-value');
    const param6Value = document.getElementById('param6-value');
    const saveButton = document.getElementById('save-button');

    function updateParamValue(slider, valueSpan) {
        valueSpan.textContent = slider.value;
    }

    // パラメータ取得とスライダーの初期化
    fetch('/getParameters')
    .then(response => response.json())
    .then(data => {
        param1Slider.value = data.param1;
        param2Slider.value = data.param2;
        param3Slider.value = data.param3;
        param4Slider.value = data.param4;
        param5Slider.value = data.param5;
        param6Slider.value = data.param6;
        updateParamValue(param1Slider, param1Value);
        updateParamValue(param2Slider, param2Value);
        updateParamValue(param3Slider, param3Value);
        updateParamValue(param4Slider, param4Value);
        updateParamValue(param5Slider, param5Value);
        updateParamValue(param6Slider, param6Value);
    })
    .catch((error) => {
        console.error('Error:', error);
        alert('Error loading parameters');
    });

    param1Slider.addEventListener('input', () => updateParamValue(param1Slider, param1Value));
    param2Slider.addEventListener('input', () => updateParamValue(param2Slider, param2Value));
    param3Slider.addEventListener('input', () => updateParamValue(param3Slider, param3Value));
    param4Slider.addEventListener('input', () => updateParamValue(param4Slider, param4Value));
    param5Slider.addEventListener('input', () => updateParamValue(param5Slider, param5Value));
    param6Slider.addEventListener('input', () => updateParamValue(param6Slider, param6Value));

    saveButton.addEventListener('click', function() {
        const params = {
            param1: param1Slider.value,
            param2: param2Slider.value,
            param3: param3Slider.value,
            param4: param4Slider.value,
            param5: param5Slider.value,
            param6: param6Slider.value
        };

        fetch('/save-params', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(params),
        })
        .then(response => response.text())
        .then(data => {
            alert('Parameters saved successfully!');
        })
        .catch((error) => {
            console.error('Error:', error);
            alert('Error saving parameters');
        });
    });
});