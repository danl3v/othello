(define my-caar
  (lambda (L)
    (car (car L))))

(define my-list
  (lambda (x . y)
    (cons x y)))

(define my-length
  (lambda (L)
    (my-length-helper L 0)))

(define my-length-helper 
  (lambda (L x)
    (if (null? L)
        x
        (my-length-helper (cdr L) (+ x 1)))))

(define my-list-tail
  (lambda (L x)
    (if (= x 0)
        L
        (my-list-tail (cdr L) (- x 1)))))

(define my-member ;(member 'a '(1 2 3 a b c)) => (a b c)
  (lambda (x L)   ;(member a '(1 2 3 a b c)) => Error
    (if (eq? '() L)
        #f
        (if (eq? x (car L))
            L
            (my-member x (cdr L))))))

(define my-assq
  (lambda (x L)
    (if (eq? '() L)
        #f
        (if (eq? x (my-caar L))
            (car L)
            (my-assq x (cdr L))))))

(define my-map ;we can make this support multiple lists?
  (lambda (f L)
    (if (null? L)
        L
        (cons (f (car L)) (my-map f (cdr L))))))

(define my-filter
  (lambda (f L)
    (if (null? L)
        L
        (if (f (car L))
            (cons (car L) (my-filter f (cdr L)))
            (my-filter f  (cdr L))))))
(define my-reverse
  (lambda (L)
    (my-reverse-helper L '())))

(define my-reverse-helper
  (lambda (L result)
    (if (null? L)
        result
        (my-reverse-helper (cdr L) (cons (car L) result)))))