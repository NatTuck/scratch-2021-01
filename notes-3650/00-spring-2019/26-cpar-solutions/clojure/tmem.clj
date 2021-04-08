
(def n (ref 0))

(defn incn []
  (dosync
   (alter n inc)))

(dorun (dosync (apply pcalls (repeat 10000 incn))))

(println @n)

(shutdown-agents)
