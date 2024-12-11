import { TC } from "./tC.js";
import { M2, m3, M3, P4, P5, m6, M6, m7, M7 } from "./intervals.js";

export const majScale = {
    intervals: [M2, M3, P4, P5, M6, M7],
    get(root) {
        let out = [];
        let direction = "ascending";
        out.push(root);
        this.intervals.forEach(int => {
            out.push(TC.getInterval(root, int, direction))
        })
        return out;
    }
}
export const minScale = {
    intervals: [M2, m3, P4, P5, m6, m7],
    get(root) {
        let out = [];
        let direction = "ascending";
        out.push(root);
        this.intervals.forEach(int => {
            out.push(TC.getInterval(root, int, direction))
        })
        return out;
    }
}

export const minHScale = {
    intervals: [M2, m3, P4, P5, m6, M7],
    get(root) {
        let out = [];
        let direction = "ascending";
        out.push(root);
        this.intervals.forEach(int => {
            out.push(TC.getInterval(root, int, direction))
        })
        return out;
    }
}

export const majHScale = {
    intervals: [M2, M3, P4, P5, m6, M7],
    get(root) {
        let out = [];
        let direction = "ascending";
        out.push(root);
        this.intervals.forEach(int => {
            out.push(TC.getInterval(root, int, direction))
        })
        return out;
    }
}

export const minMScale = {
    intervals: [M2, m3, P4, P5, M6, M7],
    get(root) {
        let out = [];
        let direction = "ascending";
        out.push(root);
        this.intervals.forEach(int => {
            out.push(TC.getInterval(root, int, direction))
        })
        return out;
    }
}

export const scaleToString = (scale) => {
    let str = "";
    scale.forEach(note => {
        str += note + " ";
    })
    return str;
}

export const displayIn = (scale, dom) => {
    dom.textContent = scale;
}