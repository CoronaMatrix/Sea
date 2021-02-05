# Sea Programming language (currently in development)

#Scopes
```
{
    let x = 3;
    {
        let y = 2;
        {
            let z = 4;
            let w = z;
            print z;
        }
    }
    let z  = 1;
    print z;
    print x;
    
}
```

#IF-ELSE

```
let percentage = 40;

if(percentage > 30){
    if(percentage == 30){
        print 30;
    }else{
        if(percentage == 40){
            print 40;
        }else{
            if(percentage == 50){
                print 50;
            }else{
                print 60;
            }
        }
    }
}else{
    print 20;
}
```
# Finding factorials upto 10 

```
let upto = 0;
while(upto < 11){
    let fact = 1;
    let i = upto;
    while(i > 1){
       fact = fact * i;
       i = i - 1;
    }
    print fact;
    upto = upto + 1;
}
```

#Finding Fibonacci numbers upto 30
```
let upto = 0;

let fibPrev = 1;
let fibNext = 1;
while(upto <= 30){
    if(upto > 1){
        fibNext = fibPrev + fibNext;
        fibPrev = fibNext;
    }
    print fibNext;
    upto = upto+1;
}
```
