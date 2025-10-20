use std::collections::HashMap;

pub fn count(nucleotide: char, dna: &str) -> Result<usize, char> {
    let res = nucleotide_counts(dna)?;

    match res.get(&nucleotide) {
        Some(&x) => Ok(x),
        None => Err(nucleotide),
    }
}

pub fn nucleotide_counts(dna: &str) -> Result<HashMap<char, usize>, char> {
    let mut hash_map: HashMap<char, usize> =
        [('A', 0usize), ('C', 0usize), ('G', 0usize), ('T', 0usize)]
            .iter()
            .cloned()
            .collect();

    let mut failed = ' ';
    dna.chars().for_each(|c| match c {
        'A' | 'C' | 'G' | 'T' => {
            hash_map.entry(c).and_modify(|e| *e += 1);
        }
        _ => {
            failed = c;
        }
    });

    if ' ' != failed {
        return Err(failed);
    }
    Ok(hash_map)
}
