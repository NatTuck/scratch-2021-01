// Strict mode:
//  - Some common code smell becomes errors.
//    - e.g. Default-global assignments (forgot to declare variable).
//  - Disables some undesirable features.
//  - Makes sure some optimizations are enabled.
//  - This is block ({}'s) scoped.
// https://johnresig.com/blog/ecmascript-5-strict-mode-json-and-more/

// All code is in an anonymous function to avoid polluting
// the global scope with function names.

(function () {
    "use strict";
    
    function onButtonPress() {
        var ps = document.getElementsByTagName("p");
        // ps is an array like object, can't index it
        ps = Array.from(ps);
        for (var ii = 0; ii < ps.length; ++ii) {
            ps[ii].innerText = "hello #" + ii;
        }
    }

    function init() {
        var btn = document.getElementById("hello-btn");
        btn.addEventListener("click", onButtonPress);
    }

    window.addEventListener("load", init, false);
})();

