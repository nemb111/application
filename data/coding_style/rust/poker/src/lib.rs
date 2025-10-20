use std::cmp::Reverse;

/// Given a list of poker hands, return a list of those hands which win.
///
/// Note the type signature: this function should return _the same_ reference to
/// the winning hand(s) as were passed in, not reconstructed strings which happen to be equal.
pub fn winning_hands<'a>(hands: &[&'a str]) -> Option<Vec<&'a str>> {
    let mut ranked_hands = hands
        .iter()
        .map(|el| {
            (
                *el,
                Hand::new(&mut el.split_ascii_whitespace().collect::<Vec<_>>()),
            )
        })
        .collect::<Vec<_>>();

    ranked_hands.sort_by(|el1, el2| el1.1.partial_cmp(&el2.1).expect("None was returned."));

    Some(
        ranked_hands
            .iter()
            .rev()
            .take_while(|el| el.1 >= ranked_hands.last().unwrap().1)
            .map(|el| el.0)
            .collect(),
    )
}

#[derive(PartialEq, PartialOrd, Debug)]
struct Hand {
    poker_hand: (PokerHand, Vec<u8>),
}

impl<'a> Hand {
    fn new(cards: &mut [&'a str]) -> Self {
        Hand {
            poker_hand: Self::evaluate_cards(cards),
        }
    }

    pub fn transform_non_number_cards(number: &str) -> &str {
        match number {
            "J" => "11",
            "Q" => "12",
            "K" => "13",
            "A" => "14",
            _ => number,
        }
    }

    fn evaluate_cards(cards: &mut [&'a str]) -> (PokerHand, Vec<u8>) {
        let (numbers, suits): (Vec<_>, Vec<_>) =
            cards.iter_mut().map(|s| s.split_at(s.len() - 1)).unzip();

        let mut numbers = numbers
            .into_iter()
            .map(|num| Self::transform_non_number_cards(num))
            .map(|num| num.parse::<u8>().unwrap())
            .collect::<Vec<u8>>();
        numbers.sort_unstable_by_key(|&el| Reverse(el));

        if let Some(ranked_cards) = Self::is_straight_flush(numbers.clone(), &suits) {
            return ranked_cards;
        } else if let Some(ranked_cards) = Self::is_four_of_a_kind(numbers.clone()) {
            return ranked_cards;
        } else if let Some(ranked_cards) = Self::is_full_house(numbers.clone()) {
            return ranked_cards;
        } else if let Some(ranked_cards) = Self::is_flush(numbers.clone(), &suits) {
            return ranked_cards;
        } else if let Some(ranked_cards) = Self::is_straight(numbers.clone()) {
            return ranked_cards;
        } else if let Some(ranked_cards) = Self::is_three_of_a_kind(numbers.clone()) {
            return ranked_cards;
        } else if let Some(ranked_cards) = Self::is_two_pair(numbers.clone()) {
            return ranked_cards;
        } else if let Some(ranked_cards) = Self::is_one_pair(numbers.clone()) {
            return ranked_cards;
        } else if let Some(ranked_cards) = Self::is_high_card(numbers) {
            return ranked_cards;
        }

        unreachable!();
    }

    fn is_straight_flush<I1, I2>(numbers: I1, suits: I2) -> Option<(PokerHand, Vec<u8>)>
    where
        I1: IntoIterator<Item = u8> + Clone,
        I2: IntoIterator<Item = &'a &'a str> + Copy,
    {
        if Self::is_flush(numbers.clone(), suits).is_some() {
            if let Some((_, res)) = Self::is_straight(numbers) {
                return Some((PokerHand::StraightFlush, res));
            }
        }

        None
    }

    fn is_four_of_a_kind<I>(numbers: I) -> Option<(PokerHand, Vec<u8>)>
    where
        I: IntoIterator<Item = u8>,
    {
        let mut numbers_coll = numbers.into_iter().collect::<Vec<_>>();

        if numbers_coll[0] == numbers_coll[1]
            && numbers_coll[0] == numbers_coll[2]
            && numbers_coll[0] == numbers_coll[3]
        {
            return Some((PokerHand::FourOfAKind, numbers_coll));
        } else if numbers_coll[1] == numbers_coll[2]
            && numbers_coll[1] == numbers_coll[3]
            && numbers_coll[1] == numbers_coll[4]
        {
            numbers_coll.rotate_left(1);
            return Some((PokerHand::FourOfAKind, numbers_coll));
        }

        None
    }

    fn is_full_house<I>(numbers: I) -> Option<(PokerHand, Vec<u8>)>
    where
        I: IntoIterator<Item = u8>,
    {
        let mut numbers_coll = numbers.into_iter().collect::<Vec<_>>();

        if numbers_coll[0] == numbers_coll[1]
            && numbers_coll[0] == numbers_coll[2]
            && numbers_coll[3] == numbers_coll[4]
        {
            return Some((PokerHand::FullHouse, numbers_coll));
        } else if numbers_coll[0] == numbers_coll[1]
            && numbers_coll[2] == numbers_coll[3]
            && numbers_coll[2] == numbers_coll[4]
        {
            numbers_coll.rotate_left(2);
            return Some((PokerHand::FullHouse, numbers_coll));
        }

        None
    }

    fn is_flush<I1, I2>(numbers: I1, suits: I2) -> Option<(PokerHand, Vec<u8>)>
    where
        I1: IntoIterator<Item = u8>,
        I2: IntoIterator<Item = &'a &'a str> + Copy,
    {
        let all_suits = suits
            .into_iter()
            .all(|sui| sui == suits.into_iter().next().unwrap());

        if all_suits {
            return Some((PokerHand::Flush, numbers.into_iter().collect()));
        }

        None
    }

    fn is_straight<I>(numbers: I) -> Option<(PokerHand, Vec<u8>)>
    where
        I: IntoIterator<Item = u8>,
    {
        let mut numbers_coll: Vec<u8> = numbers.into_iter().collect();

        // Handle case in which the straight consists of A, 2, 3, 4, 5
        if numbers_coll[0] == Self::transform_non_number_cards("A").parse().unwrap()
            && numbers_coll[4] == 2
        {
            numbers_coll[0] = 1;
            numbers_coll.rotate_left(1);
        }

        for i in 0..numbers_coll.len() - 1 {
            if numbers_coll[i].wrapping_sub(numbers_coll[i + 1]) != 1 {
                return None;
            }
        }

        Some((PokerHand::Straight, numbers_coll))
    }

    fn is_three_of_a_kind<I>(numbers: I) -> Option<(PokerHand, Vec<u8>)>
    where
        I: IntoIterator<Item = u8>,
    {
        let mut numbers_coll = numbers.into_iter().collect::<Vec<_>>();

        if numbers_coll[0] == numbers_coll[1] && numbers_coll[0] == numbers_coll[2] {
            return Some((PokerHand::ThreeOfAKind, numbers_coll));
        } else if numbers_coll[1] == numbers_coll[2] && numbers_coll[1] == numbers_coll[3] {
            numbers_coll.swap(0, 3);
            return Some((PokerHand::ThreeOfAKind, numbers_coll));
        } else if numbers_coll[2] == numbers_coll[3] && numbers_coll[2] == numbers_coll[4] {
            numbers_coll.swap(0, 3);
            numbers_coll.swap(1, 4);
            return Some((PokerHand::ThreeOfAKind, numbers_coll));
        }

        None
    }

    fn is_two_pair<I>(numbers: I) -> Option<(PokerHand, Vec<u8>)>
    where
        I: IntoIterator<Item = u8>,
    {
        let mut numbers_coll = numbers.into_iter().collect::<Vec<_>>();

        if numbers_coll[0] == numbers_coll[1] && numbers_coll[2] == numbers_coll[3] {
            return Some((PokerHand::TwoPair, numbers_coll));
        } else if numbers_coll[1] == numbers_coll[2] && numbers_coll[3] == numbers_coll[4] {
            numbers_coll.rotate_left(1);
            return Some((PokerHand::TwoPair, numbers_coll));
        } else if numbers_coll[0] == numbers_coll[1] && numbers_coll[3] == numbers_coll[4] {
            numbers_coll.swap(2, 4);
            return Some((PokerHand::TwoPair, numbers_coll));
        }

        None
    }

    fn is_one_pair<I>(numbers: I) -> Option<(PokerHand, Vec<u8>)>
    where
        I: IntoIterator<Item = u8>,
    {
        let mut numbers_coll = numbers.into_iter().collect::<Vec<_>>();

        if numbers_coll[0] == numbers_coll[1] {
            return Some((PokerHand::OnePair, numbers_coll));
        } else if numbers_coll[1] == numbers_coll[2] {
            numbers_coll.swap(0, 2);
            return Some((PokerHand::OnePair, numbers_coll));
        } else if numbers_coll[2] == numbers_coll[3] {
            numbers_coll.swap(0, 2);
            numbers_coll.swap(1, 3);
            return Some((PokerHand::OnePair, numbers_coll));
        } else if numbers_coll[3] == numbers_coll[4] {
            numbers_coll.rotate_right(2);
            return Some((PokerHand::OnePair, numbers_coll));
        }

        None
    }

    fn is_high_card<I>(numbers: I) -> Option<(PokerHand, Vec<u8>)>
    where
        I: IntoIterator<Item = u8>,
    {
        Some((PokerHand::HighCard, numbers.into_iter().collect()))
    }
}

#[derive(PartialEq, PartialOrd, Debug)]
enum PokerHand {
    HighCard,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    Straight,
    Flush,
    FullHouse,
    FourOfAKind,
    StraightFlush,
}
