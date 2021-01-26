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

  function init() {
    var hello = document.getElementById('hello');
    hello.innerText = "hello there";
  }

  // Delay the setup code until page is fully loaded.
  window.addEventListener('load', init, false);

  // Immediately call function.
})();


