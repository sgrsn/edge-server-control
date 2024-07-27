let joystickLeft, joystickRight;

function createJoystick(container, color, isLeft) {
    return new VirtualJoystick({
        container: container,
        strokeStyle: color,
        limitStickTravel: true,
        stickRadius: 60,
        stickCenterX: 0,
        stickCenterY: 0,
        preventDefault: true
    });
}

function setupJoysticks() {
    const container = document.getElementById("controls");

    joystickLeft = createJoystick(container, 'red', true);
    joystickRight = createJoystick(container, 'blue', false);

    joystickLeft.addEventListener('touchStartValidation', (event) => validateJoystickTouch(event, true));
    joystickRight.addEventListener('touchStartValidation', (event) => validateJoystickTouch(event, false));
}

function validateJoystickTouch(event, isLeft) {
    const touchY = event.changedTouches[0].pageY;
    const touchX = event.changedTouches[0].pageX;
    const screenHeight = window.innerHeight;
    const screenWidth = window.innerWidth;

    return touchY > screenHeight * 0.2 && 
           ((isLeft && touchX < screenWidth / 2) || (!isLeft && touchX >= screenWidth / 2));
}

setupJoysticks();