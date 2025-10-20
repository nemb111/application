use std::collections::HashMap;

#[derive(Debug)]
pub enum Category {
    Ones,
    Twos,
    Threes,
    Fours,
    Fives,
    Sixes,
    FullHouse,
    FourOfAKind,
    LittleStraight,
    BigStraight,
    Choice,
    Yacht,
}

type Dice = [u8; 5];

fn same_number_category(dice: Dice, category: u8) -> u8 {
    let num = category + 1;

    num * dice.into_iter().filter(|&die| die == num).count() as u8
}

fn straights(dice: Dice, category: Category) -> u8 {
    let mut sorted_dice = dice;
    sorted_dice.sort();
    match category {
        Category::LittleStraight => {
            if sorted_dice == [1, 2, 3, 4, 5] {
                30
            } else {
                0
            }
        }
        Category::BigStraight => {
            if sorted_dice == [2, 3, 4, 5, 6] {
                30
            } else {
                0
            }
        }
        _ => 0,
    }
}

fn fullhouse(dice: Dice) -> u8 {
    let mut map = HashMap::new();

    for die in dice {
        *map.entry(die).or_insert(0) += 1;
    }

    if map.len() == 2 && map.values().all(|&count| count == 2 || count == 3) {
        dice.into_iter().sum()
    } else {
        0
    }
}

fn four_of_a_kind(dice: Dice) -> u8 {
    let mut map = HashMap::new();

    for die in dice {
        *map.entry(die).or_insert(0) += 1;
    }

    if map.len() == 1 {
        let value = *map.keys().next().unwrap();
        value * 4
    } else if map.len() == 2 && map.values().all(|&count| count == 1 || count == 4) {
        let four_of_a_kind_value = *map.iter().find(|&(_, &count)| count == 4).unwrap().0;
        four_of_a_kind_value * 4
    } else {
        0
    }
}

pub fn score(dice: Dice, category: Category) -> u8 {
    match category {
        Category::Ones
        | Category::Twos
        | Category::Threes
        | Category::Fours
        | Category::Fives
        | Category::Sixes => same_number_category(dice, category as u8),
        Category::FullHouse => fullhouse(dice),
        Category::FourOfAKind => four_of_a_kind(dice),
        Category::LittleStraight | Category::BigStraight => straights(dice, category),
        Category::Choice => dice.into_iter().sum(),
        Category::Yacht => {
            if dice.iter().sum::<u8>() == 5 * dice[0] {
                50
            } else {
                0
            }
        }
    }
}
