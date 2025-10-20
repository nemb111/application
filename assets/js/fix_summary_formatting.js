
document.addEventListener("DOMContentLoaded", function () {
    // Find all <summary> elements on the page
    document.querySelectorAll("summary").forEach(summary => {
        // Find all <p> tags *inside* this summary
        summary.querySelectorAll("p").forEach(p => {
            // Replace each <p> with its inner text (remove tag, keep content)
            const textNode = document.createTextNode(p.textContent);
            p.replaceWith(textNode);
        });
    });
});