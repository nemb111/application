pub fn get_diamond(c: char) -> Vec<String> {
    if c == 'A' {
        return vec!["A".into()];
    }

    let n = usize::from(c as u8 - b'A');

    let iter_0_row = std::iter::once(
        std::iter::repeat(' ')
            .take(n)
            .chain(std::iter::once('A'))
            .chain(std::iter::repeat(' ').take(n))
            .collect::<String>(),
    );
    let iter_1_to_n_minus_1_row = (1..n).rev().map(|i| {
        std::iter::repeat(' ')
            .take(i)
            .chain(std::iter::once((b'A' + (n - i) as u8) as char))
            .chain(std::iter::repeat(' ').take(2 * (n - i) - 1))
            .chain(std::iter::once((b'A' + (n - i) as u8) as char))
            .chain(std::iter::repeat(' ').take(i))
            .collect::<String>()
    });

    let iter_n_row = std::iter::once(
        std::iter::once((b'A' + (n) as u8) as char)
            .chain(std::iter::repeat(' ').take(2 * n - 1))
            .chain(std::iter::once((b'A' + (n) as u8) as char))
            .collect::<String>(),
    );

    iter_0_row
        .clone()
        .chain(iter_1_to_n_minus_1_row.clone())
        .chain(iter_n_row)
        .chain(iter_1_to_n_minus_1_row.rev())
        .chain(iter_0_row)
        .collect::<Vec<String>>()
}
