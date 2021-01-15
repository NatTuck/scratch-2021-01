
; clojure -cp .
; (require 'demo)
; (time (fib 40))

(defn fib [x]
  (if (< x 2)
    1
    (+ (fib (- x 1))
       (fib (- x 2)))))

(def nums (into [] (repeat 5 38)))

; (time (map fib nums))
; (time (pmap fib nums))



