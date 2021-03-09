// Strict mode:
//  - Some common code smell becomes errors.
//    - e.g. Default-global assignments (forgot to declare variable).
//  - Disables some undesirable features.
//  - Makes sure some optimizations are enabled.
//  - This is block ({}'s) scoped.
// https://johnresig.com/blog/ecmascript-5-strict-mode-json-and-more/


// All code is in an anonymous function to avoid polluting
// the global scope with function names.
(function() {
  "use strict";

  function button_clicked() {
    var src = document.getElementById('data-src');
    var dst = document.getElementById('data-dst');
    dst.innerText = src.value;
  }

  function setup_button () {
    // "var" is old-style JS.
    // In ES6, you want "let" or "const"
    // No modern browser doesn't support ES6-style declarations.
    var btn = document.getElementById('click-me');
    btn.addEventListener('click', button_clicked, false);
  }

  // Delay the setup code until page is fully loaded.
  window.addEventListener('load', setup_button, false);

  // Immediately call function.
})();


