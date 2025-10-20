use rand::thread_rng;
use rand::Rng;

pub fn private_key(p: u64) -> u64 {
    thread_rng().sample(rand::distributions::Uniform::new(2, p))
}

pub fn public_key(p: u64, g: u64, a: u64) -> u64 {
    modular_pow(g, a, p)
}

pub fn secret(p: u64, b_pub: u64, a: u64) -> u64 {
    modular_pow(b_pub, a, p)
}

fn modular_pow(mut base: u64, mut exponent: u64, modulus: u64) -> u64 {
    if 1 == modulus {
        return 0;
    }

    let mut result = 1;
    base %= modulus;
    while exponent > 0 {
        if exponent % 2 == 1 {
            result = (result * base) % modulus;
        }
        exponent >>= 1;
        base = (base * base) % modulus;
    }

    result
}
