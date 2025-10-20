const HANDSHAKE: &[(u8, &str)] = &[
    (0b00001, "wink"),
    (0b00010, "double blink"),
    (0b00100, "close your eyes"),
    (0b01000, "jump"),
];

const REVERSE: u8 = 0b10000; // Reverse the order of the operations in the secret handshake.

fn reverse_when_needed<V: DoubleEndedIterator + 'static>(
    n: u8,
    iter: V,
) -> Box<dyn DoubleEndedIterator<Item = V::Item>> {
    if n & REVERSE != 0 {
        Box::new(iter.rev())
    } else {
        Box::new(iter)
    }
}

pub fn actions(n: u8) -> Vec<&'static str> {
    let iter = HANDSHAKE
        .iter()
        .filter_map(move |(k, v)| if n & k != 0 { Some(*v) } else { None });

    reverse_when_needed(n, iter).collect()
}
