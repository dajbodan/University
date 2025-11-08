#lang racket

(define (OperatorConvertion operator)
  (cond
    [(equal? '+ operator)   '(λ (x y s z) (x s ( y s z)))]
    [(equal? '* operator)   '(λ (x y s) (x ( y s)))]
    [(equal? '- operator)   '(λ (m n) ( (n (λ (x s z) ( x (λ (f g ) (g (f s))) (λ (g) (z)) (λ (u) (u)))) ) m))]
    [(equal? 'AND operator) '(λ (x y) (x y x))]
    [(equal? 'OR operator)  '(λ (x y) (x (λ (t f) (t)) y))]
    [(equal? 'NOT operator) '(λ (x t f) (x f t))]
   ))

(define (MacroConvertion macro)
  (cond
   [(equal? 'Y macro)    '(λ (f) ((λ (x) (f (x x))) (λ (x) (f (x x))) ))]
   [(equal? 'zero macro) '(λ (n) ( n ((λ (x) ( ( λ (t f) (f)) ))) (λ (t f)  (t)) ))]
   )
  )

(define (booleanConvertion bool)
        ( cond
             [(equal? 'T bool) '(λ (t f) (t))]
             [(equal? 'F bool) '(λ (t f) (f))]
         )
  )

(define (isMacro? input)
  (cond
    [(equal? 'Y input)    #t]
    [(equal? 'zero input) #t]
    [#t #f]
    )
  )

(define (isBoolean? input)
  (cond
    [(equal? 'T input) #t]
    [(equal? 'F input) #t]
    [#t #f]
    )
  )

(define (operator? input)
  (
   cond
    [(equal? '+ input)       #t]
    [(equal? '* input)       #t]
    [(equal? '- input)       #t]
    [(equal? 'AND input)     #t]
    [(equal? 'OR input)      #t]
    [(equal? 'NOT input)     #t]
    [#t #f]
   ))

;numbers

(define (NumberConvertion number)
  (
    cond
     [(equal? 0 number)  '(λ (s z) ( z ))]
     [(equal? 1 number)  '(λ (s z) ( s z ))]
     [(equal? 2 number)  '(λ (s z) ( s ( s z )))]
     [(equal? 3 number)  '(λ (s z) ( s ( s ( s z ))))]
     [(equal? 4 number)  '(λ (s z) ( s ( s ( s ( s z )))))]
     [(equal? 5 number)  '(λ (s z) ( s ( s ( s ( s ( s z ))))))]
     [(equal? 6 number)  '(λ (s z) ( s ( s ( s ( s ( s ( s z )))))))]
     [(equal? 7 number)  '(λ (s z) ( s ( s ( s ( s ( s ( s ( s z ))))))))]
     [(equal? 8 number)  '(λ (s z) ( s ( s ( s ( s ( s ( s ( s ( s z )))))))))]
     [(equal? 9 number)  '(λ (s z) ( s ( s ( s ( s ( s ( s ( s ( s ( s z ))))))))))]
     [(equal? 10 number) '(λ (s z) ( s ( s ( s ( s ( s ( s ( s ( s ( s ( s z )))))))))))]
   ))

(define (sizeList list )
  (
   cond 
     [(null? list)0]
     [#t (+ 1 (sizeList (cdr list)))]
   )
  )

(define (compare-parametrs list val)
  (
   if(null? list)
    #f
    (if(equal? (car list) val)
       #t
       (compare-parametrs (cdr list) val)
       )
  )
  )
    

(define (substitute list val substitution)
  (
   cond
    [(null? list) null]
    [(and (equal? (car list) 'λ) (compare-parametrs (cadr list) val)) caddr list] ; co kdyby tam bylo vice parametru, asi kvuli shadowingu?
    [(equal? (car list) 'λ) (cons 'λ ( cons (cadr list) (cons (substitute (caddr list) val substitution) null)))]
    [(list? (car list)) ( cons (substitute (car list) val substitution) (substitute (cdr list) val substitution))]    
    [(equal? (car list) val) (cons substitution (substitute (cdr list) val substitution))]
    [#t (cons (car list)   (substitute (cdr list) val substitution))]
   )
  )



(define (substituteAllParametrs list parametrs)
  ( if(or (not(equal? (car list) 'λ)) (not(null? (cdddr list))))
      (cons list parametrs)
    (cond
      [(and ( equal? ( sizeList (cadr list)) 0) (equal? (sizeList parametrs) 0)) ( cons (cddr list) null)]
      [(equal? (sizeList (cadr list)) 0) (cons (caddr list) parametrs) ] ; it means that we substituted all parametrs and have something in input 
      [(equal? (sizeList parametrs) 0)  (cons list (cons 1 null))]
      [#t (substituteAllParametrs
                  
                       ( cons
                               'λ
                                 (
                                 cons
                                     (cdr (cadr list))
                                     (cons
                                         (substitute (caddr list) (caadr list) (car parametrs ))
                                          null
                                     )
                                 )
                              )
                         
                       
                       (cdr parametrs)
                       
                  )
            
      ]
      )
  )
 )

(define (normalEvaluation list flagPrint)
  
 ( begin
    (if(equal? flagPrint 1)
     (printf "~s\n" list) '())
    ( if( null? list)
    null
    (
     if(not(list? list))
       list
      (if(list? (car list))
        (
         if(null? (cdr list)) ; if lambda has something in input
           (
                  cons (normalEvaluation ( car list) flagPrint) null
           ) 
           (
               let ([tempRes (substituteAllParametrs ( car list) (cdr list))])
                (
                  cond
                     [(equal? (cdr tempRes) '(1)) (normalEvaluation (cons (car tempRes) null) flagPrint)]
                     [(equal? (cdr tempRes) null) (normalEvaluation (caar tempRes) flagPrint)]
                     [#t (normalEvaluation  ( cons (caar tempRes) (append (cdr (car tempRes)) ( cdr  tempRes))) flagPrint)]
                 
                 )     
           )
        )
        ( if(equal? (car list) 'λ)
            (cons 'λ (cons  (cadr list)  ( cons (normalEvaluation (caddr list) flagPrint) null)))
            (cons ( car list)  (normalEvaluation (cdr list) flagPrint ))
        )
  )
 )
         )
))

(define (applicativeEvaluation list)
  (
    if(or (null? list) (not(list? list)))
      list
      (
        if(list? (car list))
          (
            if(null? (cdr list))
              (applicativeEvaluation (car list))
              (if(equal? ( normalEvaluation ( cadr list) 0) (cadr list))
                
                 (applicativeEvaluation (cons (car (substituteAllParametrs ( car list) ( cons (cadr list) null))) (cddr list)))
                 (applicativeEvaluation (car (substituteAllParametrs ( car list ) ( cons (append (applicativeEvaluation (cadr list)) (cddr list)) null))))
              )
              
             
           )
           (
              if(equal? (car list) 'λ)
                (cons 'λ ( cons ( cadr list) ( cons ( applicativeEvaluation ( caddr list)) null)))
                list
            )
       )
  )
 )

(define (cntPresent list var)
  (
    if(null? list)
      0
      (
         cond
          [(and (list? list) (equal? (car list) 'λ ))
            (if(compare-parametrs (cadr list) var)
              0
              (cntPresent (caddr list) var))]
          [(list? list) ( + (cntPresent (car list) var) (cntPresent (cdr list) var))]
          [(equal? list var) 1]
          [#t 0]
       )
      
   )
 )

(define (optimilization list)
  (
     if(null? list)
       null
       (
        if(not (list? list))
          list
        (if(and (equal? (car list) 'λ) (equal? (sizeList (cadr list)) 1))
            (let ([lastElement ( car (reverse (caddr list)))])
               (if(or (equal? lastElement null) (list? lastElement))
                  list
                  (if(equal? (cntPresent (caddr list) lastElement) 1)
                     (reverse (cdr (reverse (caddr list))))
                     list
                     )
               )
            )
            list
       )
   )
  )
  )



( define (expansion list)
   (cond 
    [(null? list) null]
    [(number? (car list)) (cons (NumberConvertion (car list)) (expansion (cdr list)))]
    [(operator? (car list)) (cons (OperatorConvertion (car list)) (expansion (cdr list)))]
    [(list? (car list)) (cons (expansion (car list)) (expansion (cdr list)))]
    [(isMacro? (car list)) (cons (MacroConvertion (car list)) (expansion(cdr list)))]
    [(isBoolean? (car list)) (cons (booleanConvertion (car list)) (expansion (cdr list)))]
    [#t (cons (car list) (expansion (cdr list)))]
    )
   )

