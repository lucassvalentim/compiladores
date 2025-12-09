fn maior(x: int, y: int) -> int {
    if x < y {
        return y;
    }
    else if x >= y{
        return x;
    }
}

fn calculadora(op: char, x: float, y: float) -> float {
    if op == '+' {
        return x + y;
    }
    else if op == '-'{
        return x - y;
    }
    else if op == '*'{
        return x * y;
    }
    else if op == '/' {
        let m: int; 
        if y == 0 {
            return 0.0;
        }
        return x / y;
    }
    else if n1 <= n2 + 1 {
        return x / y;
    }
    
    n = 0;
    
    return 0.0;
}

fn main(){ 
    let a, c: float;
    let a: char;
    a = 1.8;
    b = 7.2;
    c = a + e;
    f = nome1 * 2;

    maior(1, 2);
    maior(4, 0);

    println("{}", calculadora('*', a, b));
    println("{}", calculadora('+', a, b));
}