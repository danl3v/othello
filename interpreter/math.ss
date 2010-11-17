;; Team Othello
;; math.ss
;; November 17, 2010
    
(define >
  (lambda (x y . z)
  	(if (and (<= y x) (not (= x y)))
  	  (>helper y z)
  	  #f)))

(define >helper
  (lambda (x y)
  	(if (null? y)
  	  #t
      (if (and (<= (car y) x) (not (= x (car y))))
        (>helper (car y) (cdr y))
        #f))))
        
(define <
  (lambda (x y . z)
  	(if (and (<= x y) (not (= x y)))
  	  (<helper y z)
  	  #f)))

(define <helper
  (lambda (x y)
  	(if (null? y)
  	  #t
      (if (and (<= x (car y)) (not (= x (car y))))
        (<helper (car y) (cdr y))
        #f))))
        
(define >=
  (lambda (x y . z)
  	(if (or (not (<= x y)) (= x y))
  	  (>=helper y z)
  	  #f)))

(define >=helper
  (lambda (x y)
  	(if (null? y)
  	  #t
      (if (or (not (<= x (car y))) (= x (car y)))
        (>=helper (car y) (cdr y))
        #f))))

(define zero?
  (lambda (x)
    (= 0 x)))
         
(define positive?
  (lambda (x) 
    (> x 0)))

(define negative?
  (lambda (x) 
    (< x 0)))

(define abs ;;flex those babies!
  (lambda (x)
    (if (negative? x)
        (- 0 x)
        x)))

(define mod
  (lambda (x y)
    (if (or (positive? x) (zero? x))
        (if (< x (abs y))
            x
            (mod (- x (abs y)) y))
        (mod (+ x (abs y)) y))))

(define even?
  (lambda (x)
    (if (integer? x)
        (= (mod x 2) 0)
         "oops!")))

(define odd?
  (lambda (x)
    (if (integer? x)
        (= (mod x 2) 1)
        "oops!")))

(define floor
  (lambda (x)
    (- x (mod x 1))))

(define ceiling
  (lambda (x)
    (let ((y (floor x)))
        (if (= y x)
            x
            (+ y 1)))))

(define truncate
  (lambda (x)
    (if (positive? x)
        (floor x)
        (ceiling x))))

(define integer?
  (lambda (x)
    (= (truncate x) x)))

(define round
  (lambda (x)
    (let ((y (mod x 1)))
        (if (= y .5)
            (if (even? (+ x y))
                (+ x y)
                (- x y))
            (if (< y .5)
                (floor x)
                (ceiling x))))))

(define div
  (lambda (x y)
    (if (positive? x)
        (if (positive? y)
            (floor (/ x y))
            (- 0 (floor (/ x (abs y)))))
        (if (positive? y)
            (- 0 (ceiling (/ (abs x) y)))
            (ceiling (/ x y))))))
  

(define max
  (lambda (x . y)
    (max-helper x y)))

(define max-helper
  (lambda (x y)
    (if (null? y)
        x
        (if (< x (car y))
            (max-helper (car y) (cdr y))
            (max-helper x (cdr y))))))

(define min
  (lambda (x . y)
    (min-helper x y)))

(define min-helper
  (lambda (x y)
    (if (null? y)
        x
        (if (> x (car y))
            (min-helper (car y) (cdr y))
            (min-helper x (cdr y))))))

(define gcd
  (lambda (x . y)
    (gcd-helper x y)))

(define gcd-helper
  (lambda (x y)
    (if (null y)
        (abs x)
        (if (= (abs x) (abs (car y)))
            (gcd-helper x (cdr y))
            (if (< (abs x) (abs (car y)))
               (gcd-helper (- (abs (car y)) (abs x)) (cons x (cdr y)))
               (gcd-helper (- (abs x) (abs (car y))) y))))))
(define lcm
  (lambda (x . y)
    (lcm-helper x y)))

(define lcm-helper
  (lambda (x y)
    (if (null y)
        x
        (lcm-helper (/ (abs (* x (car y))) (gcd x (car y))) (cdr y)))))

(define expt
  (lambda (x y)
    (if (positive? y)
        (* x (expt x (- y 1)))
        (if (zero? y)
            1
            (/ (expt x (+ y 1)) x)))))