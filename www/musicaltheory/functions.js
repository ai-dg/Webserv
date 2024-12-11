class Functions {



  static entierAleatoire(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
  }

  static toggle(value) {
    return !value;
  }


  static noteAlt() {
    return Math.floor(Math.random() * (1 - 0 + 1));
  }

  static pause(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
  }


  static tools = {

    createElement(element, content) {

      element = document.createElement(element);
      if (arguments.length > 1) {
        element.innerHTML = content;
      }
      return element;

    }
  }


} export { Functions }


export function showElement(element, mode = "block") {
  element.style.display = mode;
}

export function showElements(elements, mode = "block") {
  elements.forEach(element => showElement(element, mode))
}
export function hideElement(element) {
  element.style.display = "none"
}

export function hideElements(elements) {
  elements.forEach(element => hideElement(element))
}

export function createElement(element, classArray, content) {
  content = content || ""
  element = document.createElement(element);
  classArray.forEach(classEl => {
    element.classList.add(classEl);
  });
  element.innerHTML = content;

  return element
}

export function createElementTest(element, config, content) {
  {
    content = content || ""
    element = document.createElement(element);

    for (let el in config) {
      if (el === "class") {
        if (Array.isArray(config[el])) {
          config[el].forEach(cl => {
            element.classList.add(cl)
          })
        } else
          element.classList.add(config.class)

      }
      if (el === "id")
        element.id = config.id
      else {
        element.setAttribute(el, config[el]);
      }
    }


    element.innerHTML = content;

    return element
  }
}




export function desactiveButton(button) {
  resetButton(button)
  setIsActiveFalse(button);
}

export function arrayListeners(array, handler) {
  array.forEach((button) => {
    button.addEventListener("click", handler);
  });
}


export function activeText(event) {
  event.target.classList.add("activeScale");
}


export function hideGroup(group) {

  group.forEach(element => {
    element.style.display = "none";
  });
}

export function isActiveButton(button) {
  console.log(button.getAttribute("isActive"))
  return button.getAttribute("isActive") === "true"

}

export function activeButton(button) {
  button.classList.add("active");
  setIsActiveTrue(button);
}



export function resetButton(button) {
  button.classList.remove("active");
  setIsActiveFalse(button);
}

export function resetButtonsGroup(buttonGroup) {
  buttonGroup.forEach((button) => {
    resetButton(button);
  });
}


export function resetStyleButton(button, style) {
  button.classList.remove(style);
  setIsActiveFalse(button);
}

export function resetStyleButtonsGroup(buttonGroup, style) {
  buttonGroup.forEach((button) => {
    resetStyleButton(button,style);
  });
}




function resetText(text) {
  text.classList.remove("activeScale");
  setIsActiveFalse(text);
}

export function resetTextGroup(textGroup) {

  textGroup.forEach((text) => {
    resetText(text);
  });

}

export function setButtonsClassGroupActive(event, buttonGroup) {

  resetButtonsGroup(buttonGroup);
  event.target.classList.add("active");
  setIsActiveTrue(event.target)
}


export function setButtonsClassGroupStyle(event, buttonGroup, style) {

  resetButtonsGroup(buttonGroup);
  event.target.classList.add(style);
  setIsActiveTrue(event.target)
}


function setIsActiveFalse(button) {
  button.setAttribute("isActive", "false");
}

function setIsActiveTrue(button) {
  button.setAttribute("isActive", "true");
}


export function setTextClassGroupActive(event, textGroup) {

  resetTextGroup(textGroup);
  event.target.classList.add("activeScale");
  displayModescalePickerSelectorss(event)
}


////// HANDLE CIRCLES 

export function putIntheCircleEdge(parent, childs) {
  let pos = parent.getBoundingClientRect()
  let parentPOS = {
    x: pos.x,
    y: pos.y,
    oX: 0,
    oY: 0,
    endX: pos.x + pos.width,
    endY: pos.y + pos.height,
    centerX: pos.width / 2,
    centerY: pos.height / 2
  }



  fillCircle(parentPOS, childs)


}



export function fillCircle(pos, array) {

  for (let i = 0; i < array.length; i++) {

    let alpha = Math.PI * 2 * i / array.length;
    setPosition(pos, array[i], alpha)

  }

}

export function setPosition(pos, element, alpha) {


  let X = 50 - (50 * Math.cos(alpha))
  let Y = 50 - (50 * Math.sin(alpha))

  element.style.left = String(X) + '%';
  element.style.top = String(Y) + '%';

}





