import { TC } from "./tC.js";

class Intervals {


	
       static Intervals = [
	
                {name: 'MinorSecond', steps: '2', semitones : 1, wholeTone: 0.5 , color : 'dissonant'},                    
                {name : 'MajorSecond', steps: '2', semitones : 2,  wholeTone: 1, color : 'dissonant'},                    
                {name : 'MinorThird', steps: '3', semitones : 3,  wholeTone: 1.5, color : 'dissonant'},                    
                {name : 'MajorThird', steps: '3', semitones : 4,  wholeTone: 2, color : 'dissonant'},                    
                {name : 'PerfectFourth', steps: '4', semitones : 5,  wholeTone: 2.5, color : 'dissonant'},                    
                {name : 'AugmentedFourth', steps: '4', semitones : 6,  wholeTone: 3, color : 'dissonant'},                    
                {name : 'DiminishedFifth', steps: '5', semitones : 6,  wholeTone: 3, color : 'dissonant'},                
                {name : 'PerfectFifth', steps: '5', semitones : 7 ,  wholeTone: 3.5, color : 'PerfectConsonnance'},
                {name : 'AugmentedFifth', steps: '5', semitones : 8,  wholeTone: 4, color : 'dissonant'},
                {name : 'MinorSixth', steps: '6', semitones : 8,  wholeTone: 4,  color : 'dissonant'},
                {name : 'MajorSixth', steps: '6', semitones : 9,  wholeTone: 4.5,  color : 'dissonant'},
                {name : 'MinorSeventh', steps: '7', semitones : 10,  wholeTone: 5, color : 'dissonant'},
                {name : 'MajorSeventh', steps: '7', semitones : 11,  wholeTone: 5.5, color : 'dissonant'},
                {name : 'PerfectOctave', steps: '8', semitones : 12,  wholeTone: 6, color : 'PerfectConsonnance'},

            ]


        static NaturalNotes = [1,1,0.5,1,1,1,0.5]
        static notes = [
                ['C','C#','Db','D','Eb','E','F','F#','Gb','G','G#','Ab','A#','Bb','B','B#','Cb']
        ]
        
        addSemitone(note){

                let hasAlteration = note.substr(1)

                switch(hasAlteration)
                {
                        case '#': 
                        break;

                        case 'b': 
                        break;
                        case '': 
                        break;

                }
                return note;

        }

        removeSemitone(note){

                let hasAlteration = note.substr(1)

                switch(hasAlteration)
                {
                        case '#': 
                        break;

                        case 'b': 
                        break;
                        case '': 
                        break;

                }
                return note;

        }


        
        returnOddMessage(){

                console.log('you are kidding me ! You will never use that');

        }



}export { Intervals };


/** 
 * @typedef {object} interval objet représentant un interval
 * @property {number} steps nombre de demi-tons 0.5 pour un demi, 1 pour un ton, etc...
 * @property {number} degree numbre de degrée par rapport au son fondamental
 * @property {string} name le nom abregé de l'interval
 /**



/**
 * 
 * @typedef {interval[]} IntervalArray C'est un tableau d'intervales décrivant une gamme, un mode ou un accords
 */


/**
 * Fonction qui renvoie un objet interval à partir de deux notes
 * @param {string} root fondamentale de l'accord 
 * @param {number} note deuxième note de l'interval
 * @return {interval} renvoie un interval
 */
export function getIntervalNameFromNotes(root, note){
    
    let degree = countDegrees(root,note)
    let steps = TC.ascendingSteps(root,note)
    let interval = allIntervals.find(inter => {
        return (inter.degree ===degree && inter.steps===steps)
    });

    return interval
}

/**
 * Cette fonction transforme un tableau d'interval en tableau avec les noms d'interval
 * @param {interval[]} array 
 * @return {string[]}
 */
export function getIntervalNameArray(array){
    let functions = []
    functions.push("R")
    array.forEach(inter => {
        functions.push(inter.name);
    })
    return functions
}


/**
 * Cette fonction compte le nombre de degrés qui séparent deux notes (le format des notes est le format anglo-saxon - ABCDEFG)
 * @param {string} root fondamentale de l'accord 
 * @param {number} note deuxième note de l'interval
 * @returns {number} un nombre représentant la distance entre deux notes où 1 représente la première
 */
export function countDegrees(root,note){
    

    let rootIndex = TC.musicalNotes.findIndex(i => i===root[0]) // ex C = 0;  // ex D = 1
    let noteIndex = TC.musicalNotes.findIndex(i => i===note[0]) // ex G = 4;  // ex A = 5

    let degree = noteIndex - rootIndex;
    let sign = Math.sign(degree);
    if (sign ===-1)
        degree +=7


    return degree--;
}


/**  
 * Cette fonction transforme un tableau de notes en tableau d'intervales représentant une gamme, un mode ou un accord, ou une suite de notes
 * @param {string[]} scale Représente un tableau contenant des notes de musiques au format anglosaxon (ABCDEFG)
 * @return {interval[]} Retourn un tableau des intervales en fonction du tableau passé en argument de la fonction
 */
export function getFunctionsArrayFromNotes(scale){

    
    let scaleDef = [];
    let root = scale[0];

    for(let index = 1; index< scale.length ; index ++){
        scaleDef.push(getIntervalNameFromNotes(root,scale[index]))
    }

    return scaleDef

}

export const augF = {steps : 0.5, degree : 0, name:"#F"}; 
export const m2 = {steps : 0.5, degree : 1, name:"m2"};
export const M2 = {steps : 1, degree : 1, name:"M2"};
export const AUG2 = {steps : 1.5, degree : 1, name:"#2"};
export const m3 = {steps : 1.5, degree : 2, name:"m3"};
export const M3 = {steps : 2, degree : 2, name:"M3"};
export const AUG3 = {steps : 2.5, degree : 2, name:"M3"};
export const DIM4 = {steps : 2, degree : 3, name:"b4"};
export const P4 = {steps : 2.5, degree : 3, name:"P4"};
export const AUG4 = {steps : 3, degree : 3, name:"#4"};
export const DIM5 = {steps : 3, degree : 4, name:"b5"};
export const P5 = {steps : 3.5, degree : 4, name:"P5"};
export const AUG5 = {steps : 4, degree : 4, name:"#5"};
export const m6 = {steps : 4, degree : 5, name:"m6"};
export const M6 = {steps : 4.5, degree : 5, name:"M6"};
export const DIM7 = {steps : 4.5, degree : 6, name:"bb7"};
export const m7 = {steps : 5, degree : 6, name:"m7"};
export const M7 = {steps : 5.5, degree : 6, name:"M7"};

export const allIntervals = [augF,m2,M2,AUG2,m3,M3,AUG3,DIM4,P4,AUG4,DIM5,P5,AUG5,m6,M6,DIM7,m7,M7]