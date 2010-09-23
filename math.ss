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
    (= (my-mod x 2) 0)))

(define my-odd?
  (lambda (x)
    (= (my-mod x 2) 1)))

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
        (my-floor (/ x y))
        (my-ceiling (/ x y)))))