import { notes } from "./notes.js";

class TC {




    static noteSharp = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B'];
    static noteFlat = ['C', 'Db', 'D', 'Eb', 'E', 'F', 'Gb', 'G', 'Ab', 'A', 'Bb', 'B'];
    static note_dble_sharp = ['B#', 'C#', 'Cx', 'D#', 'Dx', 'E#', 'F#', 'Fx', 'G#', 'Gx', 'A#', 'Ax']
    static note_dble_flat = ['Dbb', 'Db', 'Ebb', 'Eb', 'Fb', 'Gbb', 'Gb', 'Abb', 'Ab', 'Bbb', 'Bb', 'Cb']
    static musicalNotes = ['C', 'D', 'E', 'F', 'G', 'A', 'B']
    static altSharpNotes = ['C#', 'D#', 'F#', 'G#', 'A#']
    static altBemolNotes = ['Db', 'Eb', 'Gb', 'Ab', 'Bb']
    static musicalSteps = [1, 1, 0.5, 1, 1, 1, 0.5]
    static alteration = "sharp";
    static tonalContext = 0;
    static noteContext = 'sharp';
    static ionian = 'ionian';
    static dorian = 'dorian';
    static phrygian = 'phrygian';
    static lydian = 'lydian';
    static mixolydian = 'mixolydian';
    static eolian = 'eolian';
    static lydian = 'lydian';

    static ionianMH = 'ionian#5';
    static dorianMH = 'dorian#4';
    static phrygianMH = 'phrygian#3';
    static lydianMH = 'lydian#2';
    static mixolydianMH = 'mixolydian#1';
    static eolianMH = 'eolianM7';
    static lydianMH = 'lydianM6';

    static ionianMM = '';
    static dorianMM = '';
    static phrygianMM = '';
    static lydianMM = '';
    static mixolydianMM = 'superlocrian';
    static eolianMM = '';
    static lydianMM = '';

    //// not used for the moment
    static majorTonesTest = {
        scales: [
            ['C', 'D', 'E', 'F', 'G', 'A', 'B'],//0                               
            ['C#', 'D#', 'E#', 'F#', 'G#', 'A#', 'B#'],//1                        
            ['Db', 'Eb', 'F', 'Gb', 'Ab', 'Bb', 'C'],//2
            ['D', 'E', 'F#', 'G', 'A', 'B', 'C#'],//3
            ['D#', 'E#', 'Fx', 'G#', 'A#', 'B#', 'Cx'],//4                         
            ['Eb', 'F', 'G', 'Ab', 'Bb', 'C', 'D'],//5                            
            ['E', 'F#', 'G#', 'A', 'B', 'C#', 'D#'],//6                            
            ['F', 'G', 'A', 'Bb', 'C', 'D', 'E'],//8
            ['F#', 'G#', 'A#', 'B', 'C#', 'D#', 'E#'],//9
            ['Gb', 'Ab', 'Bb', 'Cb', 'Db', 'Eb', 'F'],//10
            ['G', 'A', 'B', 'C', 'D', 'E', 'F#'],//11
            ['G#', 'A#', 'B#', 'C#', 'D#', 'E#', 'Fx'],//12
            ['Ab', 'Bb', 'C', 'Db', 'Eb', 'F', 'G'],//13
            ['A', 'B', 'C#', 'D', 'E', 'F#', 'G#'],//14
            ['A#', 'B#', 'Cx', 'D#', 'E#', 'Fx', 'Gx'],//15
            ['Bb', 'C', 'D', 'Eb', 'F', 'G', 'A'],//16
            ['B', 'C#', 'D#', 'E', 'F#', 'G#', 'A#']//17

        ],
        function: ["Root", "2M", "3M", "P4", "P5", 'M6', 'M7']
    }
    ///////
    static majorTones = [
        ['C', 'D', 'E', 'F', 'G', 'A', 'B'],//0                               
        ['C#', 'D#', 'E#', 'F#', 'G#', 'A#', 'B#'],//1                        
        ['Db', 'Eb', 'F', 'Gb', 'Ab', 'Bb', 'C'],//2
        ['D', 'E', 'F#', 'G', 'A', 'B', 'C#'],//3
        ['D#', 'E#', 'Fx', 'G#', 'A#', 'B#', 'Cx'],//4                         
        ['Eb', 'F', 'G', 'Ab', 'Bb', 'C', 'D'],//5                            
        ['E', 'F#', 'G#', 'A', 'B', 'C#', 'D#'],//6                            
        ['F', 'G', 'A', 'Bb', 'C', 'D', 'E'],//8
        ['F#', 'G#', 'A#', 'B', 'C#', 'D#', 'E#'],//9
        ['Gb', 'Ab', 'Bb', 'Cb', 'Db', 'Eb', 'F'],//10
        ['G', 'A', 'B', 'C', 'D', 'E', 'F#'],//11
        ['G#', 'A#', 'B#', 'C#', 'D#', 'E#', 'Fx'],//12
        ['Ab', 'Bb', 'C', 'Db', 'Eb', 'F', 'G'],//13
        ['A', 'B', 'C#', 'D', 'E', 'F#', 'G#'],//14
        ['A#', 'B#', 'Cx', 'D#', 'E#', 'Fx', 'Gx'],//15
        ['Bb', 'C', 'D', 'Eb', 'F', 'G', 'A'],//16
        ['B', 'C#', 'D#', 'E', 'F#', 'G#', 'A#']//17

    ]

    static minor_second = { steps: 0.5, degree: 1 };
    static major_second = { steps: 1, degree: 1 };
    static minor_third = { steps: 1.5, degree: 2 };
    static major_third = { steps: 2, degree: 2 };
    static perfect_fourth = { steps: 2.5, degree: 3 };
    static augmented_fourth = { steps: 3, degree: 3 };
    static diminished_fifth = { steps: 3, degree: 4 };
    static perfect_fifth = { steps: 3.5, degree: 4 };
    static augmented_fifth = { steps: 4, degree: 4 };
    static minor_sixth = { steps: 4, degree: 5 };
    static major_sixth = { steps: 4.5, degree: 5 };
    static minor_seventh = { steps: 5, degree: 6 };
    static major_seventh = { steps: 5.5, degree: 6 };


    static sus_2_chord = [TC.major_second, TC.perfect_fifth];
    static major_chord = [TC.major_third, TC.perfect_fifth];
    static augmented_chord = [TC.major_third, TC.augmented_fifth];
    static minor_chord = [TC.minor_third, TC.perfect_fifth];
    static diminished_chord = [TC.minor_third, TC.diminished_fifth];
    static sus_4_chord = [TC.perfect_fourth, TC.perfect_fifth];


    static Maj7 = [...TC.major_chord, TC.major_seventh];
    static Dominant7 = [...TC.major_chord, TC.minor_seventh];
    static Minor7 = [...TC.minor_chord, TC.minor_seventh];
    static Minor7Diminished5 = [...TC.diminished_chord, TC.minor_seventh];
    static MinorMaj7 = [...TC.minor_chord, TC.major_seventh];
    static Maj6 = [...TC.major_chord, TC.major_sixth];
    static Minor6 = [...TC.minor_chord, TC.minor_sixth];

    static Harmonized_major_triad = ["", "-", "-", "", "", "-", "dim"];
    static Harmonized_minor_harmonic_triad = ["-", "dim", "aug", "-", "", "", "dim"];
    static Harmonized_major_harmonic_triad = ["", "dim", "-", "-", "7", "aug", "dim"];
    static Harmonized_minor_melodic_triad = ["-", "-", "aug", "", "", "dim", "dim"];

    static Harmonized_major_tretrad = ["Maj7", "-7", "-7", "Maj7", "7", "-7", "-7b5"];
    static Harmonized_minor_harmonic_tretrad = ["-Maj7", "-7b5", "Maj7#5", "-7", "7", "Maj7", "°7"];
    static Harmonized_major_harmonic_tretrad = ["Maj7", "-7b5", "-7", "-Maj7", "7", "Maj7#5", "°7"];
    static Harmonized_minor_melodic_tretrad = ["-Maj7", "-7", "Maj7#5", "7", "7", "-7b5", "-7b5"];

    static minorHTones = [
        ['A', 'B', 'C', 'D', 'E', 'F', 'G#'],//14
        ['A#', 'B#', 'C#', 'D#', 'E#', 'F#', 'Gx'],
        ['Bb', 'C', 'Db', 'Eb', 'F', 'Gb', 'A'],//16
        ['B', 'C#', 'D', 'E', 'F#', 'G', 'A#'],//17                           
        ['C', 'D', 'Eb', 'F', 'G', 'Ab', 'B'],//0                               
        ['C#', 'D#', 'E', 'F#', 'G#', 'A', 'B#'],//1                        
        ['Db', 'Eb', 'Fb', 'Gb', 'Ab', 'Bbb', 'C'],//2
        ['D', 'E', 'F', 'G', 'A', 'Bb', 'C#'],//3
        ['D#', 'E#', 'F#', 'G#', 'A#', 'B#', 'Cx'],//4                         
        ['Eb', 'F', 'Gb', 'Ab', 'Bb', 'Cb', 'D'],//5                            
        ['E', 'F#', 'G', 'A', 'B', 'C', 'D#'],//6                         
        ['F', 'G', 'Ab', 'Bb', 'C', 'Db', 'E'],//8
        ['F#', 'G#', 'A', 'B', 'C#', 'D', 'E#'],//9
        ['Gb', 'Ab', 'Bbb', 'Cb', 'Db', 'Eb', 'F'],//10
        ['G', 'A', 'Bb', 'C', 'D', 'Eb', 'F#'],//11
        ['G#', 'A#', 'B', 'C#', 'D#', 'E#', 'Fx'],//12
        ['Ab', 'Bb', 'Cb', 'Db', 'Eb', 'Fb', 'G'],//13
    ]

    static majorHTones = [
        ['A', 'B', 'C#', 'D', 'E', 'F', 'G#'],//14  
        ['A#', 'B#', 'Cx', 'D#', 'E#', 'F#', 'Gx'],//14  
        ['Bb', 'C', 'D', 'Eb', 'F', 'Gb', 'A'],//16
        ['B', 'C#', 'D#', 'E', 'F#', 'G', 'A#'],//17
        ['C', 'D', 'E', 'F', 'G', 'Ab', 'B'],//0                               
        ['C#', 'D#', 'E#', 'F#', 'G#', 'A', 'B#'],//1                        
        ['Db', 'Eb', 'F', 'Gb', 'Ab', 'Bbb', 'C'],//2
        ['D', 'E', 'F#', 'G', 'A', 'Bb', 'C#'],//3
        ['D#', 'E#', 'Fx', 'G#', 'A#', 'B#', 'Cx'],//4                         
        ['Eb', 'F', 'G', 'Ab', 'Bb', 'Cb', 'D'],//5                            
        ['E', 'F#', 'G#', 'A', 'B', 'C', 'D#'],//6                            
        ['F', 'G', 'A', 'Bb', 'C', 'Db', 'E'],//8
        ['F#', 'G#', 'A#', 'B', 'C#', 'D', 'E#'],//9
        ['Gb', 'Ab', 'Bb', 'Cb', 'Db', 'Eb', 'F'],//10
        ['G', 'A', 'B', 'C', 'D', 'Eb', 'F#'],//11
        ['G#', 'A#', 'B#', 'C#', 'D#', 'E#', 'Fx'],//12
        ['Ab', 'Bb', 'C', 'Db', 'Eb', 'Fb', 'G'],//13
    ]

    static majorPentatonic = [

        ['C', 'D', 'E', 'G', 'A'],//0                               
        ['C#', 'D#', 'E#', 'G#', 'A#'],//1                        
        ['Db', 'Eb', 'F', 'Ab', 'Bb'],//2
        ['D', 'E', 'F#', 'A', 'B'],//3                                                 
        ['Eb', 'F', 'G', 'Bb', 'C'],//5                            
        ['E', 'F#', 'G#', 'B', 'C#'],//6                            
        ['F', 'G', 'A', 'C', 'D'],//8
        ['F#', 'G#', 'A#', 'C#', 'D#'],//9
        ['Gb', 'Ab', 'Bb', 'Db', 'Eb'],//10
        ['G', 'A', 'B', 'D', 'E'],//11
        ['G#', 'A#', 'B#', 'D#', 'E#'],//12
        ['Ab', 'Bb', 'C', 'Eb', 'F'],//13
        ['A', 'B', 'C#', 'E', 'F#'],//14
        ['Bb', 'C', 'D', 'F', 'G'],//16
        ['B', 'C#', 'D#', 'F#', 'G#']//17
    ]

    static minorPentatonic = [
        ['A', 'C', 'D', 'E', 'G'],//0                               
        ['A#', 'C#', 'D#', 'E#', 'G#'],//1                        
        ['Bb', 'Db', 'Eb', 'F', 'Ab'],//2
        ['B', 'D', 'E', 'F#', 'A'],//3                                              
        ['C', 'Eb', 'F', 'G', 'Bb'],//5                            
        ['C#', 'E', 'F#', 'G#', 'B'],//6                            
        ['D', 'F', 'G', 'A', 'C'],//8
        ['D#', 'F#', 'G#', 'A#', 'C#'],//9
        ['Eb', 'Gb', 'Ab', 'Bb', 'Db'],//10
        ['E', 'G', 'A', 'B', 'D'],//11
        ['E#', 'G#', 'A#', 'B#', 'D#'],//12
        ['F', 'Ab', 'Bb', 'C', 'Eb'],//13
        ['F#', 'A', 'B', 'C#', 'E'],//14                              
        ['G', 'Bb', 'C', 'D', 'F'],//16
        ['G#', 'B', 'C#', 'D#', 'F#']//17
    ]

    static minorMTones = [
        ['D', 'E', 'F', 'G', 'A', 'B', 'C#'],//3
        ['D#', 'E#', 'F#', 'G#', 'A#', 'B#', 'Cx'],//4                         
        ['Eb', 'F', 'Gb', 'Ab', 'Bb', 'C', 'D'],//5                            
        ['E', 'F#', 'G', 'A', 'B', 'C#', 'D#'],//6                            
        ['F', 'G', 'Ab', 'Bb', 'C', 'D', 'E'],//8
        ['F#', 'G#', 'A', 'B', 'C#', 'D#', 'E#'],//9
        ['Gb', 'Ab', 'Bbb', 'Cb', 'Db', 'Eb', 'F'],//10
        ['G', 'A', 'Bb', 'C', 'D', 'E', 'F#'],//11
        ['G#', 'A#', 'B', 'C#', 'D#', 'E#', 'Fx'],//12
        ['Ab', 'Bb', 'Cb', 'Db', 'Eb', 'F', 'G'],//13
        ['A', 'B', 'C', 'D', 'E', 'F#', 'G#'],//14
        ['A#', 'B#', 'C#', 'D#', 'E#', 'Fx', 'Gx'],//15
        ['Bb', 'C', 'Db', 'Eb', 'F', 'G', 'A'],//16
        ['B', 'C#', 'D', 'E', 'F#', 'G#', 'A#'],//17                           
        ['C', 'D', 'Eb', 'F', 'G', 'A', 'B'],//0                               
        ['C#', 'D#', 'E', 'F#', 'G#', 'A#', 'B#'],//1                        
        ['Db', 'Eb', 'Fb', 'Gb', 'Ab', 'Bb', 'C'],//2
    ]


    static WholeToneScale = [
        ['C', 'D', 'E', 'F#', 'G#', 'A#'],//0                               
        ['C#', 'D#', 'E#', 'Fx', 'Gx', 'Ax'],//1
        ['Db', 'Eb', 'F', 'G', 'A', 'B']
    ]

    static semitoneTone = [
        ['C', 'Db', 'Eb', 'E', 'F#', 'G', 'A', 'Bb'],//0                               
        ['D', 'Eb', 'F', 'F#', 'Ab', 'A', 'B', 'C'], //1
        ['E', 'F', 'G', 'G#', 'Bb', 'B', 'Db', 'D'],
    ]

    static toneSemitone = [

    ]

    static majorModes = [
        [    //ionien
            { name: 'C', interval: 'fundamental', context: TC.ionian, tonesFromRoot: 0 },
            { name: 'D', interval: 'majorSecond', context: TC.ionian, tonesFromRoot: 1 },
            { name: 'E', interval: 'majorThird', context: TC.ionian, tonesFromRoot: 2 },
            { name: 'F', interval: 'perfectFourth', context: TC.ionian, tonesFromRoot: 2.5 },
            { name: 'G', interval: 'perfectFifth', context: TC.ionian, tonesFromRoot: 3.5 },
            { name: 'A', interval: 'majorSixth', context: TC.ionian, tonesFromRoot: 4.5 },
            { name: 'B', interval: 'majorSeventh', context: TC.ionian, tonesFromRoot: 5.5 },
            { name: 'C', interval: 'Octave', context: TC.ionian, tonesFromRoot: 6 }
        ],
        [    //dorien
            { name: 'D', interval: 'fundamental', context: TC.dorian, tonesFromRoot: 0 },
            { name: 'E', interval: 'majorSecond', context: TC.dorian, tonesFromRoot: 1 },
            { name: 'F', interval: 'minorThird', context: TC.dorian, tonesFromRoot: 1.5 },
            { name: 'G', interval: 'perfectFourth', context: TC.dorian, tonesFromRoot: 2.5 },
            { name: 'A', interval: 'perfectFifth', context: TC.dorian, tonesFromRoot: 3.5 },
            { name: 'B', interval: 'majorSixth', context: TC.dorian, tonesFromRoot: 4.5 },
            { name: 'C', interval: 'minorSeventh', context: TC.dorian, tonesFromRoot: 5 },
            { name: 'D', interval: 'Octave', context: TC.dorian, tonesFromRoot: 6 }
        ],
        [    //phrygien
            { name: 'E', interval: 'fundamental', context: TC.phrygian, tonesFromRoot: 0 },
            { name: 'F', interval: 'minorSecond', context: TC.phrygian, tonesFromRoot: 0.5 },
            { name: 'G', interval: 'minorThird', context: TC.phrygian, tonesFromRoot: 1.5 },
            { name: 'A', interval: 'perfectFourth', context: TC.phrygian, tonesFromRoot: 2.5 },
            { name: 'B', interval: 'perfectFifth', context: TC.phrygian, tonesFromRoot: 3.5 },
            { name: 'C', interval: 'minorSixth', context: TC.phrygian, tonesFromRoot: 4 },
            { name: 'D', interval: 'minorSeventh', context: TC.phrygian, tonesFromRoot: 5 },
            { name: 'E', interval: 'Octave', context: TC.phrygian, tonesFromRoot: 6 }
        ],
        [    //lydien
            { name: 'F', interval: 'fundamental', context: TC.lydian, tonesFromRoot: 0 },
            { name: 'G', interval: 'majorSecond', context: TC.lydian, tonesFromRoot: 1 },
            { name: 'A', interval: 'majorThird', context: TC.lydian, tonesFromRoot: 2 },
            { name: 'B', interval: 'augmentedFourth', context: TC.lydian, tonesFromRoot: 3 },
            { name: 'C', interval: 'perfectFifth', context: TC.lydian, tonesFromRoot: 3.5 },
            { name: 'D', interval: 'majorSixth', context: TC.lydian, tonesFromRoot: 4.5 },
            { name: 'E', interval: 'majorSeventh', context: TC.lydian, tonesFromRoot: 5.5 },
            { name: 'F', interval: 'Octave', context: TC.lydian, tonesFromRoot: 0 },
        ],
        [    //mixolydien
            { name: 'G', interval: 'fundamental', context: TC.mixolydian, tonesFromRoot: 0 },
            { name: 'A', interval: 'majorSecond', context: TC.mixolydian, tonesFromRoot: 1 },
            { name: 'B', interval: 'majorThird', context: TC.mixolydian, tonesFromRoot: 2 },
            { name: 'C', interval: 'perfectFourth', context: TC.mixolydian, tonesFromRoot: 2.5 },
            { name: 'D', interval: 'perfectFifth', context: TC.mixolydian, tonesFromRoot: 3.5 },
            { name: 'E', interval: 'majorSixth', context: TC.mixolydian, tonesFromRoot: 4.5 },
            { name: 'F', interval: 'minorSeventh', context: TC.mixolydian, tonesFromRoot: 5 },
            { name: 'G', interval: 'Octave', context: TC.mixolydian, tonesFromRoot: 6 }
        ],
        [    //aeolien
            { name: 'A', interval: 'fundamental', context: TC.eolian, tonesFromRoot: 0 },
            { name: 'B', interval: 'majorSecond', context: TC.eolian, tonesFromRoot: 1 },
            { name: 'C', interval: 'minorThird', context: TC.eolian, tonesFromRoot: 1.5 },
            { name: 'D', interval: 'perfectFourth', context: TC.eolian, tonesFromRoot: 2.5 },
            { name: 'E', interval: 'perfectFifth', context: TC.eolian, tonesFromRoot: 3.5 },
            { name: 'F', interval: 'minorSixth', context: TC.eolian, tonesFromRoot: 4 },
            { name: 'G', interval: 'minorSeventh', context: TC.eolian, tonesFromRoot: 5 },
            { name: 'A', interval: 'Octave', context: TC.eolian, tonesFromRoot: 6 }
        ],
        [    //locrien
            { name: 'B', interval: 'fundamental', context: TC.locrian, tonesFromRoot: 0 },
            { name: 'C', interval: 'minorSecond', context: TC.locrian, tonesFromRoot: 0.5 },
            { name: 'D', interval: 'minorThird', context: TC.locrian, tonesFromRoot: 1.5 },
            { name: 'E', interval: 'perfectFourth', context: TC.locrian, tonesFromRoot: 2.5 },
            { name: 'F', interval: 'diminishedFifth', context: TC.locrian, tonesFromRoot: 3 },
            { name: 'G', interval: 'minorSixth', context: TC.locrian, tonesFromRoot: 4 },
            { name: 'A', interval: 'minorSeventh', context: TC.locrian, tonesFromRoot: 5 },
            { name: 'B', interval: 'Octave', context: TC.locrian, tonesFromRoot: 6 }
        ]
    ]

    static minorHarmonicModes = [
        [    //aeolien7Maj
            { name: 'A', interval: 'fundamental', context: TC.eolianMH, tonesFromRoot: 0 },
            { name: 'B', interval: 'majorSecond', context: TC.eolianMH, tonesFromRoot: 1 },
            { name: 'C', interval: 'minorThird', context: TC.eolianMH, tonesFromRoot: 1.5 },
            { name: 'D', interval: 'perfectFourth', context: TC.eolianMH, tonesFromRoot: 2.5 },
            { name: 'E', interval: 'perfectFifth', context: TC.eolianMH, tonesFromRoot: 3.5 },
            { name: 'F', interval: 'minorSixth', context: TC.eolianMH, tonesFromRoot: 4 },
            { name: 'G#', interval: 'majorSeventh', context: TC.eolianMH, tonesFromRoot: 5.5 },
            { name: 'A', interval: 'Octave', context: TC.eolianMH, tonesFromRoot: 6 }
        ],
        [    //locrien6Maj
            { name: 'B', interval: 'fundamental', context: TC.locrianMH, tonesFromRoot: 0 },
            { name: 'C', interval: 'minorSecond', context: TC.locrianMH, tonesFromRoot: 0.5 },
            { name: 'D', interval: 'minorThird', context: TC.locrianMH, tonesFromRoot: 1.5 },
            { name: 'E', interval: 'perfectFourth', context: TC.locrianMH, tonesFromRoot: 2.5 },
            { name: 'F', interval: 'diminishedFifth', context: TC.locrianMH, tonesFromRoot: 3 },
            { name: 'G#', interval: 'majorSixth', context: TC.locrianMH, tonesFromRoot: 4.5 },
            { name: 'A', interval: 'minorSeventh', context: TC.locrianMH, tonesFromRoot: 5 },
            { name: 'B', interval: 'Octave', context: TC.locrianMH, tonesFromRoot: 6 }
        ],
        [    //ionienAugFifth
            { name: 'C', interval: 'fundamental', context: TC.ionianMH, tonesFromRoot: 0 },
            { name: 'D', interval: 'majorSecond', context: TC.ionianMH, tonesFromRoot: 1 },
            { name: 'E', interval: 'majorThird', context: TC.ionianMH, tonesFromRoot: 2 },
            { name: 'F', interval: 'perfectFourth', context: TC.ionianMH, tonesFromRoot: 2.5 },
            { name: 'G#', interval: 'augmentedFifth', context: TC.ionianMH, tonesFromRoot: 4 },
            { name: 'A', interval: 'majorSixth', context: TC.ionianMH, tonesFromRoot: 4.5 },
            { name: 'B', interval: 'majorSeventh', context: TC.ionianMH, tonesFromRoot: 5.5 },
            { name: 'C', interval: 'Octave', context: TC.ionianMH, tonesFromRoot: 6 }
        ],
        [    //dorienAugFourth
            { name: 'D', interval: 'fundamental', context: TC.dorianMH, tonesFromRoot: 0 },
            { name: 'E', interval: 'majorSecond', context: TC.dorianMH, tonesFromRoot: 1 },
            { name: 'F', interval: 'minorThird', context: TC.dorianMH, tonesFromRoot: 1.5 },
            { name: 'G#', interval: 'augmentedFourth', context: TC.dorianMH, tonesFromRoot: 3 },
            { name: 'A', interval: 'perfectFifth', context: TC.dorianMH, tonesFromRoot: 3.5 },
            { name: 'B', interval: 'majorSixth', context: TC.dorianMH, tonesFromRoot: 4.5 },
            { name: 'C', interval: 'minorSeventh', context: TC.dorianMH, tonesFromRoot: 5 },
            { name: 'D', interval: 'Octave', context: TC.dorianMH, tonesFromRoot: 6 }
        ],
        [    //phrygienMajThird
            { name: 'E', interval: 'fundamental', context: TC.phrygianMH, tonesFromRoot: 0 },
            { name: 'F', interval: 'minorSecond', context: TC.phrygianMH, tonesFromRoot: 0.5 },
            { name: 'G#', interval: 'majorThird', context: TC.phrygianMH, tonesFromRoot: 2 },
            { name: 'A', interval: 'perfectFourth', context: TC.phrygianMH, tonesFromRoot: 2.5 },
            { name: 'B', interval: 'perfectFifth', context: TC.phrygianMH, tonesFromRoot: 3.5 },
            { name: 'C', interval: 'minorSixth', context: TC.phrygianMH, tonesFromRoot: 4 },
            { name: 'D', interval: 'minorSeventh', context: TC.phrygianMH, tonesFromRoot: 5 },
            { name: 'E', interval: 'Octave', context: TC.phrygianMH, tonesFromRoot: 6 }
        ],
        [    //lydienAugmentedSecond
            { name: 'F', interval: 'fundamental', context: TC.lydianMH, tonesFromRoot: 0 },
            { name: 'G#', interval: 'augmentedSecond', context: TC.lydianMH, tonesFromRoot: 1.5 },
            { name: 'A', interval: 'majorThird', context: TC.lydianMH, tonesFromRoot: 2 },
            { name: 'B', interval: 'augmentedFourth', context: TC.lydianMH, tonesFromRoot: 3 },
            { name: 'C', interval: 'perfectFifth', context: TC.lydianMH, tonesFromRoot: 3.5 },
            { name: 'D', interval: 'majorSixth', context: TC.lydianMH, tonesFromRoot: 4.5 },
            { name: 'E', interval: 'majorSeventh', context: TC.lydianMH, tonesFromRoot: 5.5 },
            { name: 'F', interval: 'Octave', context: TC.lydianMH, tonesFromRoot: 6 },
        ],
        [    //mixolydianAugmentedRoot
            { name: 'G#', interval: 'fundamental', context: TC.mixolydianMH, tonesFromRoot: 0 },
            { name: 'A', interval: 'minorSecond', context: TC.mixolydianMH, tonesFromRoot: 0.5 },
            { name: 'B', interval: 'minorThird', context: TC.mixolydianMH, tonesFromRoot: 1.5 },
            { name: 'C', interval: 'diminishedFourth', context: TC.mixolydianMH, tonesFromRoot: 2 },
            { name: 'D', interval: 'diminishedFifth', context: TC.mixolydianMH, tonesFromRoot: 3 },
            { name: 'E', interval: 'minorSixth', context: TC.mixolydianMH, tonesFromRoot: 4 },
            { name: 'F', interval: 'diminishedSeventh', context: TC.mixolydianMH, tonesFromRoot: 4.5 },
            { name: 'G#', interval: 'Octave', context: TC.mixolydianMH, tonesFromRoot: 6 }
        ],
    ]

    static minorMelodicModes = [
        [   //aeolien7Maj
            { name: 'A', interval: 'fundamental', context: TC.eolianMM, tonesFromRoot: 0 },
            { name: 'B', interval: 'majorSecond', context: TC.eolianMM, tonesFromRoot: 1 },
            { name: 'C', interval: 'minorThird', context: TC.eolianMM, tonesFromRoot: 1.5 },
            { name: 'D', interval: 'perfectFourth', context: TC.eolianMM, tonesFromRoot: 2.5 },
            { name: 'E', interval: 'perfectFifth', context: TC.eolianMM, tonesFromRoot: 3.5 },
            { name: 'F#', interval: 'minorSixth', context: TC.eolianMM, tonesFromRoot: 4.5 },
            { name: 'G#', interval: 'majorSeventh', context: TC.eolianMM, tonesFromRoot: 5.5 },
            { name: 'A', interval: 'Octave', context: TC.eolianMM, tonesFromRoot: 6 }
        ],
        [   //locrien6Maj
            { name: 'B', interval: 'fundamental', context: TC.locrianMM, tonesFromRoot: 0 },
            { name: 'C', interval: 'minorSecond', context: TC.locrianMM, tonesFromRoot: 0.5 },
            { name: 'D', interval: 'minorThird', context: TC.locrianMM, tonesFromRoot: 1.5 },
            { name: 'E', interval: 'perfectFourth', context: TC.locrianMM, tonesFromRoot: 2.5 },
            { name: 'F#', interval: 'perfectFifth', context: TC.locrianMM, tonesFromRoot: 3.5 },
            { name: 'G#', interval: 'majorSixth', context: TC.locrianMM, tonesFromRoot: 4.5 },
            { name: 'A', interval: 'minorSeventh', context: TC.locrianMM, tonesFromRoot: 5 },
            { name: 'B', interval: 'Octave', context: TC.locrianMM, tonesFromRoot: 6 }
        ],
        [   //ionienAugFifth
            { name: 'C', interval: 'fundamental', context: TC.ionianMM, tonesFromRoot: 0 },
            { name: 'D', interval: 'majorSecond', context: TC.ionianMM, tonesFromRoot: 1 },
            { name: 'E', interval: 'majorThird', context: TC.ionianMM, tonesFromRoot: 2 },
            { name: 'F#', interval: 'augmentedFourth', context: TC.ionianMM, tonesFromRoot: 3 },
            { name: 'G#', interval: 'augmentedFifth', context: TC.ionianMM, tonesFromRoot: 4 },
            { name: 'A', interval: 'majorSixth', context: TC.ionianMM, tonesFromRoot: 4.5 },
            { name: 'B', interval: 'majorSeventh', context: TC.ionianMM, tonesFromRoot: 5.5 },
            { name: 'C', interval: 'Octave', context: TC.ionianMM, tonesFromRoot: 6 }
        ],
        [   //dorienAugFourth
            { name: 'D', interval: 'fundamental', context: TC.dorianMM, tonesFromRoot: 0 },
            { name: 'E', interval: 'majorSecond', context: TC.dorianMM, tonesFromRoot: 1 },
            { name: 'F#', interval: 'majorThird', context: TC.dorianMM, tonesFromRoot: 2 },
            { name: 'G#', interval: 'augmentedFourth', context: TC.dorianMM, tonesFromRoot: 3 },
            { name: 'A', interval: 'perfectFifth', context: TC.dorianMM, tonesFromRoot: 3.5 },
            { name: 'B', interval: 'majorSixth', context: TC.dorianMM, tonesFromRoot: 4.5 },
            { name: 'C', interval: 'minorSeventh', context: TC.dorianMM, tonesFromRoot: 5 },
            { name: 'D', interval: 'Octave', context: TC.dorianMM, tonesFromRoot: 6 }
        ],
        [   //phrygienMajThird
            { name: 'E', interval: 'fundamental', context: TC.phrygianMM, tonesFromRoot: 0 },
            { name: 'F#', interval: 'majorSecond', context: TC.phrygianMM, tonesFromRoot: 0.5 },
            { name: 'G#', interval: 'majorThird', context: TC.phrygianMM, tonesFromRoot: 2 },
            { name: 'A', interval: 'perfectFourth', context: TC.phrygianMM, tonesFromRoot: 2.5 },
            { name: 'B', interval: 'perfectFifth', context: TC.phrygianMM, tonesFromRoot: 3.5 },
            { name: 'C', interval: 'minorSixth', context: TC.phrygianMM, tonesFromRoot: 4 },
            { name: 'D', interval: 'minorSeventh', context: TC.phrygianMM, tonesFromRoot: 5 },
            { name: 'E', interval: 'Octave', context: TC.phrygianMM, tonesFromRoot: 6 }
        ],
        [   //lydienAugmentedSecond
            { name: 'F#', interval: 'fundamental', context: TC.lydianMM, tonesFromRoot: 0 },
            { name: 'G#', interval: 'majorSecond', context: TC.lydianMM, tonesFromRoot: 1 },
            { name: 'A', interval: 'minorThird', context: TC.lydianMM, tonesFromRoot: 1.5 },
            { name: 'B', interval: 'perfectFourth', context: TC.lydianMM, tonesFromRoot: 2.5 },
            { name: 'C', interval: 'diminishedFifth', context: TC.lydianMM, tonesFromRoot: 3 },
            { name: 'D', interval: 'minorSixth', context: TC.lydianMM, tonesFromRoot: 4 },
            { name: 'E', interval: 'minorSeventh', context: TC.lydianMM, tonesFromRoot: 5 },
            { name: 'F', interval: 'Octave', context: TC.lydianMM, tonesFromRoot: 6 },
        ],
        [   //superlocrian
            { name: 'G#', interval: 'fundamental', context: TC.mixolydianMM, tonesFromRoot: 0 },
            { name: 'A', interval: 'minorSecond', context: TC.mixolydianMM, tonesFromRoot: 0.5 },
            { name: 'B', interval: 'minorThird', context: TC.mixolydianMM, tonesFromRoot: 1.5 },
            { name: 'C', interval: 'diminishedFourth', context: TC.mixolydianMM, tonesFromRoot: 2 },
            { name: 'D', interval: 'diminishedFifth', context: TC.mixolydianMM, tonesFromRoot: 3 },
            { name: 'E', interval: 'minorSixth', context: TC.mixolydianMM, tonesFromRoot: 4 },
            { name: 'F#', interval: 'minorSeventh', context: TC.mixolydianMM, tonesFromRoot: 5 },
            { name: 'G#', interval: 'Octave', context: TC.mixolydianMM, tonesFromRoot: 6 }
        ],
    ]

    static flatTonalities = [this.majorTones[0], //C
    this.majorTones[8], //F
    this.majorTones[16], //Bb
    this.majorTones[5], //Eb
    this.majorTones[13],// Ab
    this.majorTones[2], //Db
    this.majorTones[10], //Gb
    this.majorTones[19], //Cb
    this.majorTones[7], //Fb
    ]

    static sharpTonalities = [this.majorTones[0], //C
    this.majorTones[11], //G
    this.majorTones[3], //D
    this.majorTones[14], //A
    this.majorTones[6],//E
    this.majorTones[17], //B
    this.majorTones[10], //F#
    this.majorTones[1], //C#
    this.majorTones[12], //G#
    ]

    static chromaticSharpWheel = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B', 'C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']
    static chromaticFlatWheel = ['C', 'Db', 'D', 'Eb', 'E', 'F', 'Gb', 'G', 'Ab', 'A', 'Bb', 'B', 'C', 'Db', 'D', 'Eb', 'E', 'F', 'Gb', 'G', 'Ab', 'A', 'Bb', 'B']
    static chromaticDoubleSharpWheel = ['B#', 'C#', 'Cx', 'D#', 'Dx', 'E#', 'F#', 'Fx', 'G#', 'Gx', 'A#', 'Ax', 'B#', 'C#', 'Cx', 'D#', 'Dx', 'E#', 'F#', 'Fx', 'G#', 'Gx', 'A#', 'Ax', 'B#', 'C#', 'Cx', 'D#', 'Dx', 'E#', 'F#', 'Fx', 'G#', 'Gx', 'A#', 'Ax']
    static chromaticDoubleFlatWheel = ['Dbb', 'Db', 'Ebb', 'Eb', 'Fb', 'Gbb', 'Gb', 'Abb', 'Ab', 'Bbb', 'Bb', 'Cb', 'Dbb', 'Db', 'Ebb', 'Eb', 'Fb', 'Gbb', 'Gb', 'Abb', 'Ab', 'Bbb', 'Bb', 'Cb', 'Dbb', 'Db', 'Ebb', 'Eb', 'Fb', 'Gbb', 'Gb', 'Abb', 'Ab', 'Bbb', 'Bb', 'Cb']
    static degreesCount = ['C', 'D', 'E', 'F', 'G', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'A', 'B']



    static noteEquivalance = [{ chromaticSharp: 'C', chromaticFlat: 'C', chromaticDoubleSharp: 'B#', chromaticDoubleFlat: 'Dbb', midi: [0, 12, 24, 36, 48, 60, 72, 84, 96, 108, 120] },
    { chromaticSharp: 'C#', chromaticFlat: 'Db', chromaticDoubleSharp: 'C#', chromaticDoubleFlat: 'Db', midi: [1, 13, 25, 37, 49, 61, 73, 85, 97, 109, 121] },
    { chromaticSharp: 'D', chromaticFlat: 'D', chromaticDoubleSharp: 'Cx', chromaticDoubleFlat: 'Ebb', midi: [2, 14, 26, 38, 50, 62, 74, 86, 98, 110, 122] },
    { chromaticSharp: 'D#', chromaticFlat: 'Eb', chromaticDoubleSharp: 'D#', chromaticDoubleFlat: 'Eb', midi: [3, 15, 27, 39, 51, 63, 75, 87, 99, 111, 123] },
    { chromaticSharp: 'E', chromaticFlat: 'E', chromaticDoubleSharp: 'Dx', chromaticDoubleFlat: 'Fb', midi: [4, 16, 28, 40, 52, 64, 76, 88, 100, 112, 124] },
    { chromaticSharp: 'F', chromaticFlat: 'F', chromaticDoubleSharp: 'E#', chromaticDoubleFlat: 'Gbb', midi: [5, 17, 29, 41, 53, 65, 77, 89, 101, 113, 125] },
    { chromaticSharp: 'F#', chromaticFlat: 'Gb', chromaticDoubleSharp: 'F#', chromaticDoubleFlat: 'Gbb', midi: [6, 18, 30, 42, 54, 66, 78, 90, 102, 114, 126] },
    { chromaticSharp: 'G', chromaticFlat: 'G', chromaticDoubleSharp: 'Fx', chromaticDoubleFlat: 'Abb', midi: [7, 19, 31, 43, 55, 67, 79, 91, 103, 115, 127] },
    { chromaticSharp: 'G#', chromaticFlat: 'Ab', chromaticDoubleSharp: 'G#', chromaticDoubleFlat: 'Ab', midi: [8, 20, 32, 44, 56, 68, 80, 92, 104, 116, null] },
    { chromaticSharp: 'A', chromaticFlat: 'A', chromaticDoubleSharp: 'Gx', chromaticDoubleFlat: 'Bbb', midi: [9, 21, 33, 45, 57, 69, 81, 93, 105, 117, null] },
    { chromaticSharp: 'A#', chromaticFlat: 'Ab', chromaticDoubleSharp: 'A#', chromaticDoubleFlat: 'Bb', midi: [10, 22, 34, 46, 58, 70, 82, 94, 106, 118, null] },
    { chromaticSharp: 'B', chromaticFlat: 'B', chromaticDoubleSharp: 'Ax', chromaticDoubleFlat: 'Cb', midi: [11, 23, 35, 47, 59, 71, 83, 95, 107, 119, null] },
    ]

    // pas encore utilisé
    static intervals = [
        { name: "minorSecond", steps: 0.5, degree: 1 },
        { name: "majorSecond", steps: 1, degree: 1 },
        { name: "minorThird", steps: 1.5, degree: 2 },
        { name: "majorThird", steps: 2, degree: 2 },
        { name: "perfectFourth", steps: 2.5, degree: 3 },
        { name: "augmentedFourth", steps: 3, degree: 3 },
        { name: "diminishedFourth", steps: 3, degree: 4 },
        { name: "perfectFifth", steps: 3.5, degree: 4 },
        { name: "augmentedFifth", steps: 4, degree: 4 },
        { name: "minorSixth", steps: 4, degree: 5 },
        { name: "majorSixth", steps: 4.5, degree: 5 },
        { name: "minorSeventh", steps: 5, degree: 6 },
        { name: "majorSeventh", steps: 5.5, degree: 6 },
        { name: "perfectOctave", steps: 6, degree: 7 }


    ]






    static getInterval(note, interval, direction) {


        direction = direction || 'descending';
        var octaveRange = 1;
        var range = octaveRange * 12;

        if (direction === 'ascending' || direction === 'default') {

            let indexRoot = this.findIndex(note);


            let responseIndex = indexRoot + (interval.steps * 2);

            let noteToEvaluate = this.chromaticSharpWheel[responseIndex];
            let expectedIndexNote = this.degreesCount.indexOf(note.substring(0, 1)) + interval.degree
            let expectedNote = this.degreesCount[expectedIndexNote];

            return this.eval(noteToEvaluate, expectedNote);
        } else if (direction === 'descending') {

            let indexRoot = this.findIndex(note) + range;
            let responseIndex = indexRoot - ((interval.steps * 2));
            let noteToEvaluate = this.chromaticSharpWheel[responseIndex];

            let expectedIndexNote = 7 + this.degreesCount.indexOf(note.substring(0, 1)) - interval.degree;
            let expectedNote = this.degreesCount[expectedIndexNote];

            return this.eval(noteToEvaluate, expectedNote);

        }

    }

    static findIndex(note) {
        let hasAlt = note.substring(1);
        let indexNote;
        switch (hasAlt) {
            case 'b': indexNote = this.chromaticFlatWheel.indexOf(note); break;
            case '#': if (note === 'E#' || note === 'B#') { indexNote = this.chromaticDoubleSharpWheel.indexOf(note) }
            else { indexNote = this.chromaticSharpWheel.indexOf(note) }; break;
            case 'x': indexNote = this.chromaticDoubleSharpWheel.indexOf(note); break;
            case 'bb': if (note === 'Fb' || note === 'Cb') { indexNote = this.chromaticDoubleFlatWheel.indexOf(note) }
            else { indexNote = this.chromaticSharpWheel.indexOf(note) }; break;
            case '': indexNote = this.chromaticSharpWheel.indexOf(note); break;
        }

        return indexNote;

    }


    static ascendingSteps(note1, note2) {


        let hasAlt1 = note1.substring(1);
        let hasAlt2 = note2.substring(1);
        let indexNote1;
        let indexNote2;

        switch (hasAlt1) {
            case 'b': indexNote1 = this.chromaticFlatWheel.indexOf(note1); break;
            case '#': if (note1 === 'E#' || note1 === 'B#') { indexNote1 = this.chromaticDoubleSharpWheel.indexOf(note1) }
            else { indexNote1 = this.chromaticSharpWheel.indexOf(note1) }; break;
            case 'x': indexNote1 = this.chromaticDoubleSharpWheel.indexOf(note1); break;
            case 'bb': if (note1 === 'Fb' || note1 === 'Cb') { indexNote1 = this.chromaticDoubleFlatWheel.indexOf(note1) }
            else { indexNote1 = this.chromaticSharpWheel.indexOf(note1) }; break;
            case '': indexNote1 = this.chromaticSharpWheel.indexOf(note1); break;
        }

        switch (hasAlt2) {

            case 'b': indexNote2 = this.chromaticFlatWheel.indexOf(note2); break;
            case '#': if (note2 === 'E#' || note2 === 'B#') { indexNote2 = this.chromaticDoubleSharpWheel.indexOf(note2) }
            else { indexNote2 = this.chromaticSharpWheel.indexOf(note2) }; break;
            case 'x': indexNote2 = this.chromaticDoubleSharpWheel.indexOf(note2); break;
            case 'bb': if (note2 === 'Fb' || note2 === 'Cb') { indexNote2 = this.chromaticDoubleFlatWheel.indexOf(note2) }
            else { indexNote2 = this.chromaticSharpWheel.indexOf(note2) }; break;
            case '': indexNote2 = this.chromaticSharpWheel.indexOf(note2); break;

        }

        if (indexNote1 > indexNote2)
            indexNote2 += 12;

        return (indexNote2 - indexNote1) * 0.5;


    }

    static descendingSteps(note1, note2) {
        return (6 - (this.ascendingSteps(note1, note2)));
    }

    static eval(noteToEvaluate, expectedNoteName) {

        let index = this.findIndex(noteToEvaluate);
        if (this.chromaticSharpWheel[index].substring(0, 1) === expectedNoteName) {
            return this.chromaticSharpWheel[index];
        } else if (this.chromaticFlatWheel[index].substring(0, 1) === expectedNoteName) {
            return this.chromaticFlatWheel[index];
        } else if (this.chromaticDoubleFlatWheel[index].substring(0, 1) === expectedNoteName) {
            return this.chromaticDoubleFlatWheel[index];
        } else if (this.chromaticDoubleSharpWheel[index].substring(0, 1) === expectedNoteName) {
            return this.chromaticDoubleSharpWheel[index];
        }


    }

    static toString() {

        console.log("Theorical Constants");

    }


    static noteNameGenerator(index, alt) {

        let get = notes.get;

        var noteIndex = index % 12;
        let note;
        if (alt === 'sharp') {
            note = this.noteSharp[noteIndex];
        }
        else if (alt === 'flat') {
            note = this.noteFlat[noteIndex];
        }
        else if (alt === 'bb') {

            note = this.chromaticDoubleFlatWheel[noteIndex];
        }
        else if (alt === 'x') {
            note = this.chromaticDoubleSharpWheel[noteIndex];
        }

        return get(note);

    }


    static midiToFrequency(midiNote) {

        return (440 / 32) * (2 ** ((midiNote - 9) / 12));

    }

} export { TC };