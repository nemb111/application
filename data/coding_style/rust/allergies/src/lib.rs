pub struct Allergies {
    items: Vec<Allergen>,
}

#[derive(Debug, Copy, Clone, PartialEq)]
pub enum Allergen {
    Eggs,
    Peanuts,
    Shellfish,
    Strawberries,
    Tomatoes,
    Chocolate,
    Pollen,
    Cats,
}

fn int_to_allergen(enum_val: u32) -> Option<Allergen> {
    match enum_val {
        1 => Some(Allergen::Eggs),
        2 => Some(Allergen::Peanuts),
        4 => Some(Allergen::Shellfish),
        8 => Some(Allergen::Strawberries),
        16 => Some(Allergen::Tomatoes),
        32 => Some(Allergen::Chocolate),
        64 => Some(Allergen::Pollen),
        128 => Some(Allergen::Cats),
        _ => None,
    }
}

impl Allergies {
    pub fn new(score: u32) -> Self {
        let mut items: Vec<Allergen> = Vec::with_capacity(8);

        let mask = 0b1000_0000;

        for i in 0..=7 {
            if let Some(n) = int_to_allergen(score & (mask >> i)) {
                items.push(n);
            }
        }

        Allergies { items }
    }

    pub fn is_allergic_to(&self, allergen: &Allergen) -> bool {
        self.items.contains(allergen)
    }

    pub fn allergies(&self) -> Vec<Allergen> {
        self.items.clone()
    }
}
