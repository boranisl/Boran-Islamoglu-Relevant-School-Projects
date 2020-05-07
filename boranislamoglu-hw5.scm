(define symbol-length (lambda (inSym) (if (symbol? inSym) (string-length (symbol->string inSym)) 0 ) ))

(define sequence? (lambda (inFunc) 
(if 
(or (null? inFunc) (eq? (symbol-length (car inFunc)) 1))
(if (or (null? inFunc) (and (eq? (symbol-length (car inFunc)) 1) (null? (cdr inFunc)))) #t (sequence? (cdr inFunc))) #f)))

(define same-sequence?
(lambda (inSeq1 inSeq2)
(if (and (sequence? inSeq1) (sequence? inSeq2))
(if (and (not (and (not (null? inSeq1)) (null? inSeq2))) (not (and (null? inSeq1) (not (null? inSeq2)))) (or (and (null? inSeq1) (null? inSeq2)) (eq? (car inSeq1) (car inSeq2))))
(if (or (and (null? inSeq1) (null? inSeq2))(and (null? (cdr inSeq1)) (null? (cdr inSeq2)))) #t 
(same-sequence? (cdr inSeq1) (cdr inSeq2))
) 
#f)
(error "Both of the formal parameters have to be a sequence.")
)
)
)

(define reverse-sequence
(lambda (inSeq)
(if (sequence? inSeq) 
(if (null? inSeq)
'()
(append (reverse-sequence (cdr inSeq)) (list (car inSeq))))
(error "The formal parameter has to be a sequence.")
)
)
)

(define palindrome?
(lambda (inSeq)
(if (sequence? inSeq) 
(if (equal? inSeq (reverse-sequence inSeq)) #t #f)
(error "The formal parameter has to be a sequence.")
)
)
)

(define member?
(lambda (inSym inSeq)
(if (and (symbol? inSym) (sequence? inSeq))
(if (not (null? inSeq))
(if (equal? inSym (car inSeq))
#t
(if (null? (cdr inSeq))
#f
(member? inSym (cdr inSeq))
)
)
#f
)
(error "type error")
)
)
)

(define remove-member
(lambda (inSym inSeq)
(if (and (symbol? inSym) (sequence? inSeq) (member? inSym inSeq) )
(if (equal? inSym (car inSeq))
(cdr inSeq)
(cons (car inSeq) (remove-member inSym (cdr inSeq)))
)
(error "type or membership error")
)
)
)

 
(define anagram?
(lambda (inSeq1 inSeq2)
(if (and (sequence? inSeq1) (sequence? inSeq2))
(if (and (not (and (not (null? inSeq1)) (null? inSeq2))) (not (and (null? inSeq1) (not (null? inSeq2)))) (or (and (null? inSeq1) (null? inSeq2)) (member? (car inSeq1) inSeq2)))
(if (or (and (null? inSeq1) (null? inSeq2))(and (null? (cdr inSeq1)) (null? (cdr inSeq2)))) #t (anagram? (cdr inSeq1) (remove-member (car inSeq1) inSeq2)))
#f
)
(error "both of the formal parameters must be a sequence.")
)
)
)


(define anapoli?
(lambda (inSeq1 inseq2)
(if (and (sequence? inSeq1) (sequence? inSeq2))
(if (and (anagram? inSeq1 inSeq2) (palindrome? inSeq2)) 
#t
#f
)
(error "both of the formal paramaters must be a sequence.")
)
)
)

