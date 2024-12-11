import { m3, P5 } from "./intervals.js"
import { majScale, minScale, majHScale, minHScale, minMScale } from "./scales.js";
import { TC } from "./tC.js"



export const getRelativeMinor = (root) => {
    return TC.getInterval(root, m3, "descending");
}
export const getFifthUp = (root) => {
    return TC.getInterval(root, P5, "ascending");
}
export const getFifthDown = (root) => {
    return TC.getInterval(root, P5, "descending");
}
export const getTritone = (root) => {
    return TC.getInterval(root, P5, "descending");
}



export const getHarmScale = (root, type, quality) => {

    let qual = [];
    let f;
    switch (type) {
        case 'maj':
            qual = TC.Harmonized_major_triad;
            if (quality == 4)
                qual = TC.Harmonized_major_tretrad;
            f = majScale;
            break;
        case 'minH':
            qual = TC.Harmonized_minor_harmonic_triad
            if (quality == 4)
                qual = TC.Harmonized_minor_harmonic_tretrad;
            f = minHScale;
            break;
        case 'majH':
            qual = TC.Harmonized_major_harmonic_triad;
            if (quality == 4)
                qual = TC.Harmonized_major_harmonic_tretrad;
            f = majHScale;

            break;
        case 'minM': if (quality == 3)
            qual = TC.Harmonized_minor_melodic_triad;
            if (quality == 4)
                qual = TC.Harmonized_minor_melodic_tretrad;
            f = minMScale;
            console.log("tt", qual)
            break;
        default: qual = TC.Harmonized_major_tretrad; f = majScale;
            break;
    }

    console.log(qual)
    let scale = f.get(root);
    let out = []
    for (let i = 0; i < scale.length; i++) {
        let tmp = scale[i] + qual[i]
        out.push(tmp);
    }
    console.log(out)
    return out;
}


export const getNeighbourTones = (root) => {
    console.log(root)
    let nT = {
        fifthDown: {
            root : getFifthDown(root),
            min_root : getRelativeMinor(getFifthDown(root)),
            maj: majScale.get(getFifthDown(root)),
            min: minHScale.get(getRelativeMinor(getFifthDown(root)))
        },
        scale: {
            root,
            min_root : getRelativeMinor(root),
            maj: majScale.get(root),
            min: minHScale.get(getRelativeMinor(root))
        },
        fifthUp: {
            root : getFifthUp(root),
            min_root : getRelativeMinor(getFifthUp(root)),
            maj: majScale.get(getFifthUp(root)),
            min: minHScale.get(getRelativeMinor(getFifthUp(root)))
        }
    };
    return nT;
}

export const getNeighbourHarmonizedTones = (root, quality) => {
    let fd = getFifthDown(root);
    let fu = getFifthUp(root);


    let nT = {
        fifthDown: {
            root : fd,
            min_root : getRelativeMinor(fd),
            maj: getHarmScale(fd, "maj", quality),
            min: getHarmScale(getRelativeMinor(fd), "minH", quality)
        },
        scale: {
            root,
            min_root : getRelativeMinor(root),
            maj: getHarmScale(root, "maj", quality),
            min: getHarmScale(getRelativeMinor(root), "minH", quality)
        },
        fifthUp: {
            root : fu,
            min_root : getRelativeMinor(fu),
            maj: getHarmScale(fu, "maj", quality),
            min: getHarmScale(getRelativeMinor(fu), "minH", quality)
        }
    };
    console.log(nT)
    return nT;

}
