
document.addEventListener("DOMContentLoaded", function () {
  const images = document.querySelectorAll(".custom-readme table img");

  images.forEach(img => {
    // skip if already wrapped
    if (img.parentElement.tagName.toLowerCase() === "a") return;

    // create link to full-size image
    const link = document.createElement("a");
    link.href = img.src;        // link points to the image itself
    link.className = "dynamic-lightbox";

    // replace img with link and append img inside
    img.parentNode.replaceChild(link, img);
    link.appendChild(img);
  });
});