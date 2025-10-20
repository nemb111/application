---
# Feel free to add content and custom Front Matter to this file.
# To modify the layout, see https://jekyllrb.com/docs/themes/#overriding-theme-defaults

layout: page
---



{% capture readme_content %}
{% include_relative README.md %}
{% endcapture %}

<div class="custom-readme">
    {{ readme_content }}
</div>