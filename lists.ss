(define my-caar
  (lambda (L)
    (car (car L))))

(define my-cadr
  (lambda (L)
    (car (cdr L))))

(define my-cdar
  (lambda (L)
    (cdr (car L))))

(define my-caaar
  (lambda (L)
    (car (car (car L)))))

(define my-caadr
  (lambda (L)
    (car (car (cdr L)))))

(define my-cadar
  (lambda (L)
    (car (cdr (car L)))))

(define my-cdaar
  (lambda (L)
    (cdr (car (car L)))))

(define my-caddr
  (lambda (L)
    (car (cdr (cdr L)))))

(define my-cdadr
  (lambda (L)
    (cdr (car (cdr L)))))

(define my-cddar
  (lambda (L)
    (cdr (cdr (car L)))))

(define my-cdddr
  (lambda (L)
    (cdr (cdr (cdr L)))))

(define my-caaaar
  (lambda (L)
    (car (car (car (car L))))))

(define my-caaadr
  (lambda (L)
    (car (car (car (cdr L))))))

(define my-caadar
  (lambda (L)
    (car (car (cdr (car L))))))

(define my-cadaar
  (lambda (L)
    (car (cdr (car (car L))))))

(define my-cdaaar
  (lambda (L)
    (cdr (car (car (car L))))))

(define my-caaddr
  (lambda (L)
    (car (car (cdr (cdr L))))))

(define my-cadadr
  (lambda (L)
    (car (cdr (car (cdr L))))))

(define my-cdaadr
  (lambda (L)
    (cdr (car (car (cdr L))))))

(define my-caddar
  (lambda (L)
    (car (cdr (cdr (car L))))))

(define my-cdadar
  (lambda (L)
    (cdr (car (cdr (car L))))))

(define my-cddaar
  (lambda (L)
    (cdr (cdr (car (car L))))))

(define my-cadddr
  (lambda (L)
    (car (cdr (cdr (cdr L))))))

(define my-cdaddr
  (lambda (L)
    (cdr (car (cdr (cdr L))))))

(define my-cddadr
  (lambda (L)
    (cdr (cdr (car (cdr L))))))

(define my-cdddar
  (lambda (L)
    (cdr (cdr (cdr (car L))))))

(define my-cddddr
  (lambda (L)
    (cdr (cdr (cdr (cdr L))))))

(define my-list
  (lambda x
    x))

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

(define my-list?
  (lambda (L)
    (or (null? L) (and (pair? L) (my-list? (cdr L))))))

(define my-list-ref
  (lambda (L i)
    (if (= i 0)
        (car L)
        (my-list-ref (cdr L) (- i 1)))))

(define my-cons*
  (lambda (x . y)
    (my-cons*-helper (cdr (my-reverse (cons x y))) (car (my-reverse (cons x y))))))

(define my-cons*-helper
  (lambda (L result)
    (if (null? L)
        result
        (my-cons*-helper (cdr L) (cons (car L) result)))))
    
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

(define othello-list-tester
  (list '((my-caar '((1 2) (3 4) (5 6) (7 8))) 1)
        '((my-cadr '((1 2) (3 4) (5 6) (7 8))) '(3 4))
        '((my-cadar '((1 2 3) (4 5 6) (7 8 9))) 2)
        '((my-cdddar '((1 2 3 4) (4 5 6) (7 8))) '(4)) ;we also have all permutations up to 4 car/cdr
        '((my-list 1 2 3) '(1 2 3))
        '((my-list) '())
        '((my-length '(1 2 3 4)) 4)
        '((my-length '()) 0)
        '((my-list-tail '(0 1 2 3 4) 3) '(3 4))
        '((my-member 'a '(1 2 3 a b c)) '(a b c))
        '((my-member 'z '(1 2 3 a b c)) #f)
        '((my-assq 2 '((0 a) (1 b) (2 c) (3 d))) '(2 c))
        '((my-assq -3 '((0 a) (1 b) (2 c) (3 d))) #f)
        '((my-map (lambda (x) (* x x)) '(1 2 3)) '(1 4 9))
        '((my-append '(a b) '(c d)) '(a b c d))
        '((my-append '(a b) 'c) '(a b . c))
        '((my-reverse '(1 2 3)) '(3 2 1))
        '((my-list? (cons 1 2)) #f)
        '((my-list? (cons 1 '(2 3))) #t)
        '((my-list-ref '(1 2 3 4 5) 3) 4)
        '((my-cons* 1 2 3 '(4 5)) '(1 2 3 4 5))
        '((my-cons* 3) 3)
        '((my-memp even? '(1 2 3)) '(2 3))
        '((my-remq 2 '(1 2 3 2 4 5)) '(1 3 4 5))
        '((my-remp even? '(1 2 3 4 5)) '(1 3 5))
        '((my-filter even? '(1 2 3 4 5)) '(2 4))
        '((my-find even? '(1 2 3 4 5)) 2)
        '((my-assp even? '((1 a) (2 b) (3 c))) '(2 b))))