use std::collections::HashMap;

#[derive(Default)]
struct Outcome {
    matches_played: u32,
    win: u32,
    draw: u32,
    loss: u32,
    points: u32,
}

impl Outcome {
    fn new() -> Self {
        Default::default()
    }
}

pub fn tally(match_results: &str) -> String {
    let mut team_results: HashMap<&str, Outcome> = HashMap::new();

    match_results.lines().for_each(|line| {
        let results: Vec<&str> = line.split(';').collect();
        let outcome_first_team = team_results.entry(results[0]).or_insert_with(Outcome::new);
        update_outcomes(outcome_first_team, true, results[2]);
        let outcome_second_team = team_results.entry(results[1]).or_insert_with(Outcome::new);
        update_outcomes(outcome_second_team, false, results[2]);
    });

    create_ascii_tally(team_results)
}

fn update_outcomes(outcome: &mut Outcome, is_first_team: bool, match_result: &str) {
    enum MatchResult {
        Win,
        Loss,
        Draw,
    }

    let actual_match_result;

    match match_result.to_lowercase().as_ref() {
        "win" if is_first_team => actual_match_result = MatchResult::Win,
        "win" if !is_first_team => actual_match_result = MatchResult::Loss,
        "loss" if is_first_team => actual_match_result = MatchResult::Loss,
        "loss" if !is_first_team => actual_match_result = MatchResult::Win,
        "draw" => actual_match_result = MatchResult::Draw,
        _ => {
            panic!("This default case should never be reached");
        }
    }

    outcome.matches_played += 1;

    match actual_match_result {
        MatchResult::Win => {
            outcome.win += 1;
            outcome.points += 3;
        }
        MatchResult::Loss => {
            outcome.loss += 1;
        }
        MatchResult::Draw => {
            outcome.draw += 1;
            outcome.points += 1;
        }
    }
}

fn create_ascii_tally(team_results: HashMap<&str, Outcome>) -> String {
    let mut team_results: Vec<(&str, Outcome)> = team_results.into_iter().collect();
    team_results.sort_by(|a, b| a.1.points.cmp(&b.1.points).reverse().then(a.0.cmp(b.0)));

    let mut res = "Team                           | MP |  W |  D |  L |  P".to_string();
    for (key, value) in team_results {
        res.push_str("\n");
        res.push_str(
            format!(
                "{:30} | {:>2} | {:>2} | {:>2} | {:>2} | {:>2}",
                key, value.matches_played, value.win, value.draw, value.loss, value.points
            )
            .as_ref(),
        )
    }

    res
}
