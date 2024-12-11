// import { createElementTest } from "./functions.js"



export const noteContext = Object.freeze({
	german: "ger",
	english: "eng",
	latin: "lat"
})

export const notes = {

	context: noteContext.latin,

	notes: [
		{ eng: "", ger: "", lat: "" },
		{ eng: "Abb", ger: "Abb", lat: "Labb" },
		{ eng: "Ab", ger: "Ab", lat: "Lab" },
		{ eng: "A", ger: "A", lat: "La" },
		{ eng: "A#", ger: "A#", lat: "La#" },
		{ eng: "Ax", ger: "Ax", lat: "Lax" },
		{ eng: "Bbb", ger: "Bb", lat: "Sibb" },
		{ eng: "Bb", ger: "B", lat: "Sib" },
		{ eng: "B", ger: "H", lat: "Si" },
		{ eng: "B#", ger: "H#", lat: "Si#" },
		{ eng: "Cb", ger: "Cb", lat: "Dob" },
		{ eng: "C", ger: "C", lat: "Do" },
		{ eng: "C#", ger: "C#", lat: "Do#" },
		{ eng: "Cx", ger: "Cx", lat: "Dox" },
		{ eng: "Dbb", ger: "Dbb", lat: "Rebb" },
		{ eng: "Db", ger: "Db", lat: "Reb" },
		{ eng: "D", ger: "D", lat: "Re" },
		{ eng: "D#", ger: "D#", lat: "Re#" },
		{ eng: "Dx", ger: "Dx", lat: "Rex" },
		{ eng: "Ebb", ger: "Ebb", lat: "Mibb" },
		{ eng: "Eb", ger: "Eb", lat: "Mib" },
		{ eng: "E", ger: "E", lat: "Mi" },
		{ eng: "E#", ger: "E#", lat: "Mi#" },
		{ eng: "Fb", ger: "Fb", lat: "Fab" },
		{ eng: "F", ger: "F", lat: "Fa" },
		{ eng: "F#", ger: "F#", lat: "Fa#" },
		{ eng: "Fx", ger: "Fx", lat: "Fax" },
		{ eng: "Gbb", ger: "Gbb", lat: "Solbb" },
		{ eng: "Gb", ger: "Gb", lat: "Solb" },
		{ eng: "G", ger: "G", lat: "Sol" },
		{ eng: "G#", ger: "G#", lat: "Sol#" },
		{ eng: "Gx", ger: "Gx", lat: "Solx" },
	],

	/**
	 * Cette fonction détermine le contexte de traduction des notes de musique (eng pour english - ger pour geremand - lat pour latin)
	 * @param {string} nCtx contexte de traduction : - ("eng", "ger", "lat")
	 */
	setCtx(nCtx) {
		this.context = nCtx;
		this.resetNotesInApp()////// peut être a virer
	},

	/**
	 * Retourne le contexte de traduction des notes de musique
	 * @returns Retourne le contexte de traduction des notes de musique
	 */
	getCtx() {
		return this.context
	},

	/**
	 * Fonction servant à récupérer la traduction d'une note anglo-saxonne en fonction du contexte courant de traduction
	 * @param {string} note représente la note à traduire
	 * @returns {string} Retourne une chaine de caractère représentant la note traduite
	 */
	get(note) {

		let res = notes.notes.find(n => n["eng"] === note);
		return res[notes.context];

	},

	/**
	 * Traduit une note en notation standard anglo-saxonne et renvoie sa valeur
	 * @param {string} note Chaine de caractère représentant une note de musique
	 * @returns Retourne une note traduite en notation standard anglo-saxonne
	 */
	std(note) {
		let res = notes.notes.find(n => n[notes.context] === note);
		return res["eng"];

	},

	/**
	 * Fonction servant à traduire un tableau de notes d'une langue vers une autre langue - Ne modifie pas le tableau d'origine
	 * @param {string[]} scale Tableau représentant une gamme
	 * @param {string} from Contexte linguistique de départ ("eng", "ger", "lat")
	 * @param {string} to contexte d'arrivé ("eng", "ger", "lat")
	 * @returns Retourne un tableau qui est le résultat de la traduction de la fonction
	 */
	translateFromTo(scale, from, to) {
		let copy = [];
		let res;
		if (!to)
			to = 'eng';
		console.log('a', scale);
		console.log('b', from);
		console.log('c', to);

		for (let i = 0; i < scale.length; i++) {
			res = notes.notes.find(n => n[from] === scale[i]);
			copy.push(res[to])
		}
		return copy;
	},

	/**
	 * Cette fonction traduit un tableau de notes de l'anglo-saxon vers la langue définie dans le contexte de traduction
	 * @param {string[]} scale Tableau représentant une gamme en notation anglo-saxone
	 * @returns {string[]} Retourne un tableau traduit en fonction du contexte de traduction
	 */
	translate(scale) {
		for (let i = 0; i < scale.length; i++) {
			scale[i] = this.get(scale[i])
		}
		return scale;
	},


	/**
	 * 
	 */
	resetNotesInApp() {

		document.getElementById("scale_layer_menu").innerHTML = ""
		// let el = createElementTest("scale-picker", { class: "modePicker", toLayer: "main" });
		document.getElementById("scale_layer_menu").appendChild(el)


		//cibler tout ce qui contient des notes, bouttons, pickers,
		// 
		// reset fretboard
	}





}

export function handleNotes(event) {
	switch (event.target.value) {
		case "eng": notes.setCtx(noteContext.english); break;
		case "ger": notes.setCtx(noteContext.german); break;
		case "lat": notes.setCtx(noteContext.latin); break;
	}
}
