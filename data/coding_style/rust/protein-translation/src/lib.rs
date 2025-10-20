use std::collections::HashMap;

pub struct CodonsInfo<'a> {
    codon_protein: HashMap<&'a str, &'a str>,
}

impl<'a> CodonsInfo<'a> {
    pub fn name_for(&self, codon: &str) -> Option<&'a str> {
        self.codon_protein.get(codon).copied()
    }

    pub fn of_rna(&self, rna: &str) -> Option<Vec<&'a str>> {
        rna.chars()
            .collect::<Vec<_>>()
            .windows(3)
            .step_by(3)
            .take_while(|s| {
                self.name_for(s.iter().collect::<String>().as_str()) != Some(&"stop codon")
            })
            .map(|s| self.name_for(s.iter().collect::<String>().as_str()))
            .collect()
    }
}

pub fn parse<'a>(pairs: Vec<(&'a str, &'a str)>) -> CodonsInfo<'a> {
    CodonsInfo {
        codon_protein: pairs.into_iter().collect(),
    }
}
