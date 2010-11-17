;; Team Othello
;; lists.ss
;; November 17, 2010

(define caar
  (lambda (L)
    (car (car L))))

(define cadr
  (lambda (L)
    (car (cdr L))))

(define cdar
  (lambda (L)
    (cdr (car L))))
    
(define cddr
  (lambda (L)
    (cdr (cdr L))))
    
(define caaar
  (lambda (L)
    (car (car (car L)))))

(define caadr
  (lambda (L)
    (car (car (cdr L)))))

(define cadar
  (lambda (L)
    (car (cdr (car L)))))

(define cdaar
  (lambda (L)
    (cdr (car (car L)))))

(define caddr
  (lambda (L)
    (car (cdr (cdr L)))))

(define cdadr
  (lambda (L)
    (cdr (car (cdr L)))))

(define cddar
  (lambda (L)
    (cdr (cdr (car L)))))

(define cdddr
  (lambda (L)
    (cdr (cdr (cdr L)))))

(define caaaar
  (lambda (L)
    (car (car (car (car L))))))

(define caaadr
  (lambda (L)
    (car (car (car (cdr L))))))

(define caadar
  (lambda (L)
    (car (car (cdr (car L))))))

(define cadaar
  (lambda (L)
    (car (cdr (car (car L))))))

(define cdaaar
  (lambda (L)
    (cdr (car (car (car L))))))

(define caaddr
  (lambda (L)
    (car (car (cdr (cdr L))))))

(define cadadr
  (lambda (L)
    (car (cdr (car (cdr L))))))

(define cdaadr
  (lambda (L)
    (cdr (car (car (cdr L))))))

(define caddar
  (lambda (L)
    (car (cdr (cdr (car L))))))

(define cdadar
  (lambda (L)
    (cdr (car (cdr (car L))))))

(define cddaar
  (lambda (L)
    (cdr (cdr (car (car L))))))

(define cadddr
  (lambda (L)
    (car (cdr (cdr (cdr L))))))

(define cdaddr
  (lambda (L)
    (cdr (car (cdr (cdr L))))))

(define cddadr
  (lambda (L)
    (cdr (cdr (car (cdr L))))))

(define cdddar
  (lambda (L)
    (cdr (cdr (cdr (car L))))))

(define cddddr
  (lambda (L)
    (cdr (cdr (cdr (cdr L))))))

(define list
  (lambda x
    x))

(define length
  (lambda (L)
    (length-helper L 0)))

(define length-helper 
  (lambda (L x)
    (if (null? L)
        x
        (length-helper (cdr L) (+ x 1)))))

(define list-tail
  (lambda (L x)
    (if (= x 0)
        L
        (list-tail (cdr L) (- x 1)))))

(define member ;(member 'a '(1 2 3 a b c)) => (a b c)
  (lambda (x L)   ;(member a '(1 2 3 a b c)) => Error
    (if (null? L)
        #f
        (if (equal? x (car L))
            L
            (member x (cdr L))))))

(define assq
  (lambda (x L)
    (if (null? L)
        #f
        (if (equal? x (caar L))
            (car L)
            (assq x (cdr L))))))

(define map ;we can make this support multiple lists?
  (lambda (f L)
    (if (null? L)
        L
        (cons (f (car L)) (map f (cdr L))))))

(define append
  (lambda (L M)
    (letrec ((prepend-reversed (lambda (L M)
                                 (if (null? L)
                                     M
                                     (prepend-reversed (cdr L) (cons (car L) M))))))
      (prepend-reversed (reverse L) M))))

(define reverse
  (lambda (L)
    (reverse-helper L (quote ()))))

(define reverse-helper
  (lambda (L result)
    (if (null? L)
        result
        (reverse-helper (cdr L) (cons (car L) result)))))

(define list?
  (lambda (L)
    (or (null? L) (and (pair? L) (list? (cdr L))))))

(define list-ref
  (lambda (L i)
    (if (= i 0)
        (car L)
        (list-ref (cdr L) (- i 1)))))

(define cons*
  (lambda (x . y)
    (cons*-helper (cdr (reverse (cons x y))) (car (reverse (cons x y))))))

(define cons*-helper
  (lambda (L result)
    (if (null? L)
        result
        (cons*-helper (cdr L) (cons (car L) result)))))
    
(define memp
  (lambda (f L)
    (if (null? L)
        #f
        (if (f (car L))
            L
            (memp f (cdr L))))))

(define remq
  (lambda (x L)
    (filter (lambda (y) (not (equal? x y))) L)))

(define remp
  (lambda (f L)
    (filter (lambda (x) (not (f x))) L)))

(define filter
  (lambda (f L)
    (if (null? L)
        L
        (if (f (car L))
            (cons (car L) (filter f (cdr L)))
            (filter f  (cdr L))))))

(define find
  (lambda (f L)
    (if (null? L)
        #f
        (if (f (car L))
            (car L)
            (find f (cdr L))))))

(define assp ;; in example, list is strange
  (lambda (f L)
    (if (null? L)
        #f
        (if (f (caar L))
            (car L)
            (assp f (cdr L))))))