// Manual namespacing with anonymous scope.
// This isn't nesissary once we use a toolchain
// that provides JavaScript modules.
(function() {
    "use strict";

    function onButtonPress() {
        let ps = document.getElementsByTagName("p");
        ps = Array.from(ps);
        //ps.forEach(function(p) {
        //    p.innerText = "hello";
        //});
        alert("doing work...");

        ps.forEach((p) => {
            p.innerText = "hello";
        });
    }

    function init() {
        let btn = document.querySelector("#btn");
        btn.addEventListener("click", onButtonPress);
    }

    window.addEventListener("load", init, false);
})()
