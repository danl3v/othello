;; Team Othello
;; November 17, 2010
;; sort.ss

;; some helper functions

(define div ;; modified from math.ss
  (lambda (x y)
    (if (positive? x)
        (if (positive? y)
            (floor (/ x y))
            (- 0 (floor (/ x (abs y)))))
        (if (positive? y)
            (- 0 (ceiling (/ (abs x) y)))
            (ceiling (/ x y))))))

(define list-head
  (lambda (L n)
    (reverse (list-head-helper L n (quote ())))))

(define list-head-helper
  (lambda (L n result)
    (if (= n 0)
        result
        (list-head-helper (cdr L) (- n 1) (cons (car L) result)))))

(define list-tail
  (lambda (L n)
    (if (= n 0)
        L
        (list-tail (cdr L) (- n 1)))))

;; merge sort

(define sort
  (lambda (p L)
    (merge-sort p L)))

(define merge-sort
  (lambda (p L)
    (if (< (length L) 2)
        L
        (merge p (merge-sort p (list-head L (div (length L) 2))) (merge-sort p (list-tail L (div (length L) 2))) (quote ())))))

(define merge ;; think about non-tail recursive version first, merge with a stack, then reverse, two tail recursive ops
  (lambda (p left right result)
    (if (or (> (length left) 0) (> (length right) 0)) ;; if left or right greater than 0
        (if (and (> (length left) 0) (> (length right) 0)) ;; if both left and right greater than 0
            (if (p (list-ref left 0) (list-ref right 0)) ;; if first left bigger than first right
                (merge p (list-tail left 1) right (append result (list (list-ref left 0)))) ;; append left
                (merge p left (list-tail right 1) (append result (list (list-ref right 0))))) ;; append right
            (if (> (length left) 0) ;; if left has more than 0 elements
                (merge p (list-tail left 1) right (append result (list (list-ref left 0))))
                (merge p left (list-tail right 1) (append result (list (list-ref right 0))))))
        result)))