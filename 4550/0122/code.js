(function() {
    "use strict";

    function button_clicked() {
        let src = document.getElementById('data-src');
        let dst = document.getElementById('data-dst');
        // API is the DOM, or "Document Object Model"
        dst.innerText = src.value;
    }

    function setup_button() {
        let btn = document.getElementById('click-me');
        btn.addEventListener('click', button_clicked, false);
    }

    window.addEventListener('load', setup_button, false);

    window.NATSCODE = {
        clicked: button_clicked,
    };
})();
