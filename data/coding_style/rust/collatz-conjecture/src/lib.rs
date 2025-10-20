pub fn collatz(n: u64) -> Option<u64> {
   if 0 == n {
       return None;
   }
   
   
    let mut n = n;

    let mut it = 0;
    loop {
        if 1 == n {
            break;
        }

        if 0 == n % 2 {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
        it += 1;
    }

    Some(it)
}
