
(function () {
    "use strict";
    
    function onButtonPress() {
        var ps = document.getElementsByTagName("p");
        ps = Array.from(ps);
        //for (var ii = 0; ii < ps.length; ++ii) {
        //    ps[ii].innerText = "hello #" + ii;
        //}
        ps.forEach(function (p) {
            p.innerText = "hello";
        });
    }

    function init() {
        var btn = document.getElementById("btn");
        btn.addEventListener("click", onButtonPress);
    }

    window.addEventListener("load", init, false);
})();

