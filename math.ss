;; Team Othello
;; math.ss
;; September 24, 2010

(define my-not
  (lambda (x)
    (if x #f #t)))
    
(define my->
  (lambda (x y . z)
  	(if (and (<= y x) (my-not (= x y)))
  	  (my->helper y z)
  	  #f)))

(define my->helper
  (lambda (x y)
  	(if (null? y)
  	  #t
      (if (and (<= (car y) x) (my-not (= x (car y))))
        (my->helper (car y) (cdr y))
        #f))))
        
(define my-<
  (lambda (x y . z)
  	(if (and (<= x y) (my-not (= x y)))
  	  (my-<helper y z)
  	  #f)))

(define my-<helper
  (lambda (x y)
  	(if (null? y)
  	  #t
      (if (and (<= x (car y)) (my-not (= x (car y))))
        (my-<helper (car y) (cdr y))
        #f))))
        
(define my->=
  (lambda (x y . z)
  	(if (or (my-not (<= x y)) (= x y))
  	  (my->=helper y z)
  	  #f)))

(define my->=helper
  (lambda (x y)
  	(if (null? y)
  	  #t
      (if (or (my-not (<= x (car y))) (= x (car y)))
        (my->=helper (car y) (cdr y))
        #f))))

(define my-zero?
  (lambda (x)
    (= 0 x)))
         
(define my-positive?
  (lambda (x) 
    (> x 0)))

(define my-negative?
  (lambda (x) 
    (< x 0)))

(define my-abs ;;flex those babies!
  (lambda (x)
    (if (my-negative? x)
        (- 0 x)
        x)))

(define my-mod
  (lambda (x y)
    (if (or (my-positive? x) (my-zero? x))
        (if (< x (my-abs y))
            x
            (my-mod (- x (my-abs y)) y))
        (my-mod (+ x (my-abs y)) y))))

(define my-even?
  (lambda (x)
    (if (my-integer? x)
        (= (my-mod x 2) 0)
         "oops!")))

(define my-odd?
  (lambda (x)
    (if (my-integer? x)
        (= (my-mod x 2) 1)
        "oops!")))

(define my-floor
  (lambda (x)
    (- x (my-mod x 1))))

(define my-ceiling
  (lambda (x)
    (define y (my-floor x))
    (if (= y x)
        x
        (+ y 1))))

(define my-truncate
  (lambda (x)
    (if (my-positive? x)
        (my-floor x)
        (my-ceiling x))))

(define my-integer?
  (lambda (x)
    (= (truncate x) x)))

(define my-round
  (lambda (x)
    (define y (my-mod x 1))
    (if (= y .5)
        (if (even? (+ x y))
            (+ x y)
            (- x y))
        (if (< y .5)
            (my-floor x)
            (my-ceiling x)))))

(define my-div
  (lambda (x y)
    (if (my-positive? x)
        (if (my-positive? y)
            (my-floor (/ x y))
            (- 0 (my-floor (/ x (my-abs y)))))
        (if (my-positive? y)
            (- 0 (my-ceiling (/ (my-abs x) y)))
            (my-ceiling (/ x y))))))
  

(define my-max
  (lambda (x . y)
    (my-max-helper x y)))

(define my-max-helper
  (lambda (x y)
    (if (null? y)
        x
        (if (< x (car y))
            (my-max-helper (car y) (cdr y))
            (my-max-helper x (cdr y))))))

(define my-min
  (lambda (x . y)
    (my-min-helper x y)))

(define my-min-helper
  (lambda (x y)
    (if (null? y)
        x
        (if (> x (car y))
            (my-min-helper (car y) (cdr y))
            (my-min-helper x (cdr y))))))

(define my-gcd
  (lambda (x . y)
    (my-gcd-helper x y)))

(define my-gcd-helper
  (lambda (x y)
    (if (null y)
        (my-abs x)
        (if (= (my-abs x) (my-abs (car y)))
            (my-gcd-helper x (cdr y))
            (if (< (my-abs x) (my-abs (car y)))
               (my-gcd-helper (- (my-abs (car y)) (my-abs x)) (cons x (cdr y)))
               (my-gcd-helper (- (my-abs x) (my-abs (car y))) y))))))
(define my-lcm
  (lambda (x . y)
    (my-lcm-helper x y)))

(define my-lcm-helper
  (lambda (x y)
    (if (null y)
        x
        (my-lcm-helper (/ (my-abs (* x (car y))) (my-gcd x (car y))) (cdr y)))))

(define my-expt
  (lambda (x y)
    (if (my-positive? y)
        (* x (my-expt x (- y 1)))
        (if (my-zero? y)
            1
            (/ (my-expt x (+ y 1)) x)))))