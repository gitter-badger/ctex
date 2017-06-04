import os
import markdown

from flask import render_template
from flask import Markup
from app import app

@app.route('/')
def home():
    md_path = os.path.join(app.static_folder, 'contents')
    file_names = ['head.md', 'introduction.md', 
                  'installation.md', 
                  'usage.md', 'contributing.md', 
                  'documentation.md', 'license.md']
    data = {}
    for file_name in file_names:
        with open(os.path.join(md_path, file_name), 'r') as f:
            md = Markup(markdown.markdown(f.read()))
            title = file_name[:-3]
            data[title] = md
    return render_template('index.html', data=data)