// Bob is a lackadaisical teenager. In conversation, his responses are very limited.

// Bob answers 'Sure.' if you ask him a question, such as "How are you?".

// He answers 'Whoa, chill out!' if you YELL AT HIM (in all capitals).

// He answers 'Calm down, I know what I'm doing!' if you yell a question at him.

// He says 'Fine. Be that way!' if you address him without actually saying anything.

// He answers 'Whatever.' to anything else.

// Bob's conversational partner is a purist when it comes to written communication and always follows normal rules regarding sentence punctuation in English.

pub fn reply(message: &str) -> &str {
    let res = (
        is_question(message),
        is_yelled(message),
        is_whitespace(message),
    );

    match res {
        (_, _, true) => "Fine. Be that way!",
        (true, false, _) => "Sure.",
        (false, true, _) => "Whoa, chill out!",
        (true, true, _) => "Calm down, I know what I'm doing!",
        _ => "Whatever.",
    }
}

fn is_yelled(message: &str) -> bool {
    message.chars().any(|c| c.is_alphabetic())
        && message
            .chars()
            .filter(|c| c.is_alphabetic())
            .all(|c| c.is_uppercase())
}

fn is_question(message: &str) -> bool {
    match message.chars().filter(|c| !c.is_whitespace()).last() {
        Some('?') => true,
        _ => false,
    }
}

fn is_whitespace(message: &str) -> bool {
    message.chars().all(|c| c.is_whitespace())
}
