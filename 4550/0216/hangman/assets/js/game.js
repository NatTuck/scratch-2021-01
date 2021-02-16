
export function uniq(xs) {
    return Array.from(new Set(xs));
}

export function word_view(secret, guesses) {
    let view = [];
    for (let cc of secret.split('')) {
        if (guesses.includes(cc)) {
            view.push(cc);
        }
        else {
            view.push("_");
        }
    }
    return view;
}

export function bad_guesses(secret, guesses) {
    /*
    let bads = [];
    for (let gg in guesses) {
        if (!secret.split('').includes(gg)) {
            bads.push(gg);
        }
    }
    return uniq(bads);
    */
    let letters = secret.split('');
    return uniq(guesses.filter((x) => !letters.includes(x)));
}

export function lives_left(secret, guesses) {
    return 8 - bad_guesses(secret, guesses).length;
}
