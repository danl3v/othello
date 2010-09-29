(define my-caar
  (lambda (L)
    (car (car L))))

;; (define cadr

;; (define cddddr

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
    (if (null? L)
        #f
        (if (eq? x (car L))
            L
            (my-member x (cdr L))))))

(define my-assq
  (lambda (x L)
    (if (null? L)
        #f
        (if (eq? x (my-caar L))
            (car L)
            (my-assq x (cdr L))))))

(define my-map ;we can make this support multiple lists?
  (lambda (f L)
    (if (null? L)
        L
        (cons (f (car L)) (my-map f (cdr L))))))

(define my-append
  (lambda (L M)
    (letrec ((prepend-reversed (lambda (L M)
                                 (if (null? L)
                                     M
                                     (prepend-reversed (cdr L) (cons (car L) M))))))
      (prepend-reversed (my-reverse L) M))))

(define my-reverse
  (lambda (L)
    (my-reverse-helper L '())))

(define my-reverse-helper
  (lambda (L result)
    (if (null? L)
        result
        (my-reverse-helper (cdr L) (cons (car L) result)))))

;; (define my-list?

(define my-list-ref
  (lambda (L i)
    (if (= i 0)
        (car L)
        (my-list-ref (cdr L) (- i 1)))))

;; (define cons*

(define my-memp
  (lambda (f L)
    (if (null? L)
        #f
        (if (f (car L))
            L
            (my-memp f (cdr L))))))

(define my-remq
  (lambda (x L)
    (my-filter (lambda (y) (not (equal? x y))) L)))

(define my-remp
  (lambda (f L)
    (my-filter (lambda (x) (not (f x))) L)))

(define my-filter
  (lambda (f L)
    (if (null? L)
        L
        (if (f (car L))
            (cons (car L) (my-filter f (cdr L)))
            (my-filter f  (cdr L))))))

(define my-find
  (lambda (f L)
    (if (null? L)
        #f
        (if (f (car L))
            (car L)
            (my-find f (cdr L))))))

(define my-assp ;; in example, list is strange
  (lambda (f L)
    (if (null? L)
        #f
        (if (f (my-caar L))
            (car L)
            (my-assp f (cdr L))))))