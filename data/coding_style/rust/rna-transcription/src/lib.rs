#[derive(Debug, PartialEq)]
pub struct DNA {
    dna_sequence: String,
}

#[derive(Debug, PartialEq)]
pub struct RNA {
    rna_sequence: String,
}

fn construct_sequence(sequence: &str, nucleotides: &str) -> Result<String, usize> {
    for (idx, ch) in sequence.char_indices() {
        if !nucleotides.contains(ch) {
            return Err(idx);
        }
    }

    Ok(sequence.to_string())
}

impl DNA {
    pub fn new(dna: &str) -> Result<DNA, usize> {
        Ok(DNA {
            dna_sequence: construct_sequence(dna, "ACGT")?,
        })
    }

    pub fn into_rna(self) -> RNA {
        let res: String = self
            .dna_sequence
            .chars()
            .map(|c| match c {
                'A' => 'U',
                'C' => 'G',
                'G' => 'C',
                'T' => 'A',
                _ => c,
            })
            .collect();
        RNA { rna_sequence: res }
    }
}

impl RNA {
    pub fn new(rna: &str) -> Result<RNA, usize> {
        Ok(RNA {
            rna_sequence: construct_sequence(rna, "ACGU")?,
        })
    }
}
