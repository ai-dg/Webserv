import { resetButtonsGroup, setButtonsClassGroupActive, setButtonsClassGroupStyle, resetStyleButtonsGroup } from "./functions.js"
import { displayIn } from "./scales.js";
import { getNeighbourHarmonizedTones, getNeighbourTones } from "./tones.js";
import { scaleToString } from "./scales.js";

const selscale = [document.querySelector(".sel1"), document.querySelector(".sel2")]
const selmode = [document.querySelector(".sel3"), document.querySelector(".sel4"), document.querySelector(".sel5")]

const fdmaj = document.querySelector('.fdmaj');
const fdmin = document.querySelector('.fdmin');
const rmaj = document.querySelector('.rmaj');
const rmin = document.querySelector('.rmin');
const fumaj = document.querySelector('.fumaj');
const fumin = document.querySelector('.fumin');
const harmScale = document.querySelector('.harmScale');
const harmMinScale = document.querySelector('.harmMinScale');
const harmFuScale = document.querySelector('.harmFuScale');
const harmFuMinScale = document.querySelector('.harmFuMinScale');
const harmFdScale = document.querySelector('.harmFdScale');
const harmFdMinScale = document.querySelector('.harmFdMinScale');

const nt_notes = [...document.getElementsByClassName("nt_notes")];


export const type_info_menu = (event) => {

}

export const engage_neighboor_tones_menu = () => {

    selscale.forEach(el => {
        let context = document.querySelector(".cir")
        el.addEventListener('click', (event) => {
            resetStyleButtonsGroup(selscale,"menu_active")
            setButtonsClassGroupStyle(event, selscale, "menu_active")
            if (event.target.textContent == "Notes") {
                context.setAttribute("notes","notes")
                update_harmonized_display(getNeighbourTones(rmaj.textContent));
            }
            else {
                context.setAttribute("notes","chords")
                update_harmonized_display(getNeighbourHarmonizedTones(rmaj.textContent, 4));
            }
        })
    })

    selmode.forEach(el => {

        el.addEventListener('click', (event) => {

            resetStyleButtonsGroup(selmode, "menu_active")
            setButtonsClassGroupStyle(event, selmode, "menu_active")
        })

    })


    // let context = document.querySelector(".cir").getAttribute("notes");
    // console.log("context : " ,context);
    // if (context == "notes")
    //     document.querySelector(".cir").setAttribute("notes", "chords");
    // else
    //     document.querySelector(".cir").setAttribute("notes", "notes");



    nt_notes.forEach(note => {

        note.addEventListener('click', (event) => {
            resetButtonsGroup(nt_notes);
            setButtonsClassGroupActive(event, nt_notes);
            let context = document.querySelector(".cir").getAttribute("notes");
            // console.log("context : " ,context);
            if (context === "notes") {
                let nb = getNeighbourTones(event.target.textContent);
                update_harmonized_display(nb)
            }
            else {
                let nb = getNeighbourTones(event.target.textContent);
                update_harmonized_display(getNeighbourHarmonizedTones(nb.scale.root, 4));
            }
        })
    })
}


export const update_harmonized_display = (nb) => {

    fdmaj.innerHTML = nb.fifthDown.root;
    fdmin.innerHTML = nb.fifthDown.min_root + "m";
    rmaj.innerHTML = nb.scale.root;
    rmin.innerHTML = nb.scale.min_root + "m";
    fumaj.innerHTML = nb.fifthUp.root;
    fumin.innerHTML = nb.fifthUp.min_root + "m";

    displayIn(scaleToString(nb.scale.maj), harmScale);
    displayIn(scaleToString(nb.scale.min), harmMinScale);
    displayIn(scaleToString(nb.fifthUp.maj), harmFuScale);
    displayIn(scaleToString(nb.fifthUp.min), harmFuMinScale);
    displayIn(scaleToString(nb.fifthDown.maj), harmFdScale);
    displayIn(scaleToString(nb.fifthDown.min), harmFdMinScale);

}


