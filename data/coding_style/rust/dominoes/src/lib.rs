type Id = usize;

fn extend_chain(next_id: u8, result: &mut Vec<(Id, bool)>, input: &[(u8, u8)]) -> bool {
    for (i, (first, second)) in input.iter().enumerate() {
        if (result.contains(&(i, false)) || result.contains(&(i, true)))
            && result.len() == input.len()
        {
            // Check chain is complete
            let first_el_to_check = if result[0].1 { input[0].1 } else { input[0].0 };

            let second_el_to_check = if result.last().unwrap().1 {
                input[result.last().unwrap().0].0
            } else {
                input[result.last().unwrap().0].1
            };

            return first_el_to_check == second_el_to_check;
        }

        if result.contains(&(i, false)) || result.contains(&(i, true)) {
            continue;
        }

        if *first == next_id {
            result.push((i, false));
            if extend_chain(*second, result, input) {
                return true;
            }
            result.pop();
        } else if *second == next_id {
            result.push((i, true));
            if extend_chain(*first, result, input) {
                return true;
            }
            result.pop();
        }
    }

    false
}

fn construct_chain(input: &[(u8, u8)]) -> Option<Vec<(u8, u8)>> {
    let mut result = Vec::new();

    for (i, (first, second)) in input.iter().enumerate() {
        result.push((i, false));

        if extend_chain(*second, &mut result, input) {
            dbg!(&result);
            return Some(
                result
                    .iter()
                    .map(|(id, flipped)| {
                        if *flipped {
                            (input[*id].1, input[*id].0)
                        } else {
                            input[*id]
                        }
                    })
                    .collect::<Vec<_>>(),
            );
        }

        result.clear();
        result.push((i, true));
        if extend_chain(*first, &mut result, input) {
            dbg!(&result);
            return Some(
                result
                    .iter()
                    .map(|(id, flipped)| {
                        if *flipped {
                            (input[*id].1, input[*id].0)
                        } else {
                            input[*id]
                        }
                    })
                    .collect::<Vec<_>>(),
            );
        }
    }

    None
}

pub fn chain(input: &[(u8, u8)]) -> Option<Vec<(u8, u8)>> {
    if input.is_empty() {
        return Some(vec![]);
    }

    construct_chain(input)
}
