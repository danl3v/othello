;; Team Othello
;; standard.ss
;; November 17, 2010

(define not
  (lambda (x)
    (if x #f #t)))
    
(define null?
  (lambda (x)
    (if (equal? null x) #t #f)))