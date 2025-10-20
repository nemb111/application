document.addEventListener("DOMContentLoaded", () => {
  // Find all links in the document
  document.querySelectorAll("a").forEach(link => {
    // Check if it's a GitHub user-attachments video file
    if (link.href.includes("github.com/user-attachments/assets/") && getContentType(link.href)) {
      // Create a video element
      const video = document.createElement("video");
      video.src = link.href;
      video.controls = true;
      video.width = 640;
      video.style.display = "block";
      video.style.margin = "1rem 0";
      video.style.borderRadius = "8px";

      // Replace the link with the video
      link.parentNode.replaceChild(video, link);
    }
  });
});


async function getContentType(url) {
  try {
    const response = await fetch(url, { method: 'HEAD' });
    const type = response.headers.get('Content-Type');
    return type;
  } catch (error) {
    console.error('Failed to fetch content type:', error);
    return null;
  }
}
