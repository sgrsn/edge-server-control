(function() {
    const container = document.getElementById("controls");
    const position1 = document.getElementById("position1");
    const position2 = document.getElementById("position2");

    let leftButtonPressed = 0;
    let rightButtonPressed = 0;

    const buttonLeft = document.getElementById("button-left");
    const buttonRight = document.getElementById("button-right");

    function setupButtonListeners() {
        buttonLeft.addEventListener('touchstart', (e) => handleButtonPress(e, buttonLeft, true));
        buttonLeft.addEventListener('touchend', () => handleButtonRelease(buttonLeft, true));

        buttonRight.addEventListener('touchstart', (e) => handleButtonPress(e, buttonRight, false));
        buttonRight.addEventListener('touchend', () => handleButtonRelease(buttonRight, false));
    }

    function handleButtonPress(e, button, isLeft) {
        e.preventDefault();
        if (isLeft) leftButtonPressed = 1;
        else rightButtonPressed = 1;
        button.style.backgroundColor = 'rgba(0, 123, 255, 1)';
    }

    function handleButtonRelease(button, isLeft) {
        if (isLeft) leftButtonPressed = 0;
        else rightButtonPressed = 0;
        button.style.backgroundColor = 'rgba(0, 123, 255, 0.8)';
    }

    function preventDefaultTouchBehavior() {
        //document.body.addEventListener('touchstart', (e) => e.preventDefault(), { passive: false });
        //document.body.addEventListener('touchmove', (e) => e.preventDefault(), { passive: false });
        document.body.addEventListener('touchstart', function(e) {
            if (e.target.tagName !== 'BUTTON') {
                e.preventDefault();
            }
        }, { passive: false });

        document.body.addEventListener('touchmove', function(e) {
            if (e.target.tagName !== 'BUTTON') {
                e.preventDefault();
            }
        }, { passive: false });
    }

    function updateJoystickInfo() {
        const leftX = Math.round(joystickLeft.deltaX());
        const leftY = Math.round(joystickLeft.deltaY()) * -1;
        position1.innerHTML = `<b>Left Joystick:</b> X: ${leftX}, Y: ${leftY}`;

        const rightX = Math.round(joystickRight.deltaX());
        const rightY = Math.round(joystickRight.deltaY()) * -1;
        position2.innerHTML = `<b>Right Joystick:</b> X: ${rightX}, Y: ${rightY}`;

        sendJoystickData(leftX, leftY, rightX, rightY);
    }

    function sendJoystickData(leftX, leftY, rightX, rightY) {
        const xhr = new XMLHttpRequest();
        xhr.open('PUT', `./index.html?leftX=${leftX}&leftY=${leftY}&rightX=${rightX}&rightY=${rightY}&leftButton=${leftButtonPressed}&rightButton=${rightButtonPressed}`);
        xhr.send();
    }

    function init() {
        setupButtonListeners();
        preventDefaultTouchBehavior();
        setInterval(updateJoystickInfo, 200);
    }

    init();
})();