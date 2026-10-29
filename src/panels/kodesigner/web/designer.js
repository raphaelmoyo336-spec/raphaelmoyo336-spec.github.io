'use strict';

const runtimeError = document.getElementById('runtime-error');
let loadingProject = false;
let designerBridge = null;
let preservedDocument = {
  doctype: '<!DOCTYPE html>',
  htmlAttributes: [],
  bodyAttributes: [],
  head: [],
  baseUrl: '',
  assetReferences: {},
};

try {
  const editor = grapesjs.init({
    container: '#gjs',
    height: '100%',
    fromElement: false,
    storageManager: false,
    panels: { defaults: [] },
    blockManager: { appendTo: '#blocks' },
    layerManager: { appendTo: '#layers' },
    selectorManager: { appendTo: '#selectors' },
    styleManager: {
      appendTo: '#styles',
      sectors: [
        {
          name: 'Layout',
          open: true,
          properties: ['display', 'position', 'width', 'height', 'padding', 'margin'],
        },
        {
          name: 'Typography',
          open: false,
          properties: [
            'font-family',
            'font-size',
            'font-weight',
            'color',
            'line-height',
            'text-align',
          ],
        },
        {
          name: 'Appearance',
          open: false,
          properties: ['background-color', 'border', 'border-radius', 'box-shadow', 'opacity'],
        },
      ],
    },
    deviceManager: {
      devices: [
        { name: 'Desktop', width: '' },
        { name: 'Tablet', width: '768px', widthMedia: '992px' },
        { name: 'Mobile', width: '375px', widthMedia: '480px' },
      ],
    },
    components:
      '<main class="kd-page"><section class="kd-hero"><h1>Design your page</h1><p>Drag a component from the left, then select it to edit its styles.</p><a class="kd-button" href="#">Get started</a></section></main>',
    style:
      '.kd-page{min-height:100vh;padding:48px 24px;background:#f2f5f8;font-family:Arial,sans-serif}.kd-hero{max-width:820px;margin:40px auto;padding:52px 44px;background:#fff;border-radius:12px;box-shadow:0 12px 35px rgba(25,40,55,.12)}.kd-hero h1{margin:0 0 16px;color:#1d2b3a;font-size:42px}.kd-hero p{color:#526170;font-size:17px;line-height:1.6}.kd-button{display:inline-block;margin-top:12px;padding:12px 20px;border-radius:5px;background:#2479c5;color:#fff;text-decoration:none}',
  });
  window.koDesignerEditor = editor;

  editor.BlockManager.add('kd-section', {
    label: 'Section',
    category: 'Layout',
    content:
      '<section style="padding: 32px 24px;"><h2>Section title</h2><p>Add your content here.</p></section>',
  });
  editor.BlockManager.add('kd-columns', {
    label: 'Two columns',
    category: 'Layout',
    content:
      '<div style="display: flex; gap: 20px; padding: 24px;"><div style="flex: 1;"><h3>Column one</h3><p>Column content</p></div><div style="flex: 1;"><h3>Column two</h3><p>Column content</p></div></div>',
  });
  editor.BlockManager.add('kd-heading', {
    label: 'Heading',
    category: 'Basic',
    content: '<h2>New heading</h2>',
  });
  editor.BlockManager.add('kd-text', {
    label: 'Text',
    category: 'Basic',
    content: '<p>Write something here.</p>',
  });
  editor.BlockManager.add('kd-button', {
    label: 'Link button',
    category: 'Basic',
    content: '<a href="#" style="display: inline-block; padding: 10px 16px;">Click here</a>',
  });
  editor.BlockManager.add('kd-image', {
    label: 'Image',
    category: 'Basic',
    content:
      '<img alt="Image placeholder" src="data:image/svg+xml,%3Csvg xmlns=%22http://www.w3.org/2000/svg%22 width=%22600%22 height=%22300%22 viewBox=%220 0 600 300%22%3E%3Crect width=%22600%22 height=%22300%22 fill=%22%23e5eaf0%22/%3E%3Ctext x=%22300%22 y=%22155%22 text-anchor=%22middle%22 font-family=%22sans-serif%22 font-size=%2222%22 fill=%22%23677788%22%3EImage%3C/text%3E%3C/svg%3E" style="max-width: 100%; height: auto;">',
  });

  editor.on('update', () => {
    if (!loadingProject && designerBridge) designerBridge.documentChanged();
  });

  document.querySelectorAll('[data-command]').forEach((button) => {
    button.addEventListener('click', () => editor.runCommand(button.dataset.command));
  });

  document.getElementById('device-select').addEventListener('change', (event) => {
    editor.setDevice(event.target.value);
  });

  const exportDialog = document.getElementById('export-dialog');
  document.getElementById('export-button').addEventListener('click', () => {
    document.getElementById('html-output').value = editor.getHtml();
    document.getElementById('css-output').value = editor.getCss();
    exportDialog.hidden = false;
  });
  document.getElementById('close-export').addEventListener('click', () => {
    exportDialog.hidden = true;
  });
  document.addEventListener('keydown', (event) => {
    if (event.key === 'Escape') exportDialog.hidden = true;
  });

  window.koDesignerLoadProject = (projectData) => {
    loadingProject = true;
    editor.loadProjectData(projectData);
    preservedDocument = projectData.koDesigner || {
      doctype: '<!DOCTYPE html>',
      htmlAttributes: [],
      bodyAttributes: [],
      head: [],
      baseUrl: '',
      assetReferences: {},
    };
    applyCanvasDocumentContext();
    window.setTimeout(() => {
      loadingProject = false;
    }, 100);
    return true;
  };
  window.koDesignerGetProjectData = () => {
    const projectData = editor.getProjectData();
    projectData.koDesigner = preservedDocument;
    return JSON.stringify(projectData);
  };
  window.koDesignerImportHtml = (html, stylesheets, baseUrl, assets, assetReferences) => {
    const parsed = new DOMParser().parseFromString(html, 'text/html');
    const localStyles = [];
    const preservedHead = [];
    const rewriteCssAssets = (css) => css.replace(
      /url\(\s*(['"]?)([^'")]+)\1\s*\)/gi,
      (match, quote, reference) => assets[reference]
        ? `url("${assets[reference]}")`
        : match,
    );

    parsed.querySelectorAll('[src], [poster]').forEach((element) => {
      ['src', 'poster'].forEach((attribute) => {
        const reference = element.getAttribute(attribute);
        if (reference && assets[reference]) {
          element.setAttribute(attribute, assets[reference]);
        }
      });
    });
    parsed.querySelectorAll('[style]').forEach((element) => {
      element.setAttribute('style', rewriteCssAssets(element.getAttribute('style')));
    });

    Array.from(parsed.head.children).forEach((element) => {
      if (element.tagName === 'STYLE') {
        localStyles.push(rewriteCssAssets(element.textContent || ''));
      } else if (element.tagName === 'LINK'
        && (element.getAttribute('rel') || '').toLowerCase().split(/\s+/).includes('stylesheet')
        && Object.prototype.hasOwnProperty.call(stylesheets, element.getAttribute('href'))) {
        localStyles.push(rewriteCssAssets(stylesheets[element.getAttribute('href')]));
      } else if (element.tagName !== 'BASE') {
        preservedHead.push(element.outerHTML);
      }
    });

    preservedDocument = {
      doctype: parsed.doctype
        ? `<!DOCTYPE ${parsed.doctype.name}>`
        : '<!DOCTYPE html>',
      htmlAttributes: Array.from(parsed.documentElement.attributes)
        .map(({ name, value }) => [name, value]),
      bodyAttributes: Array.from(parsed.body.attributes)
        .map(({ name, value }) => [name, value]),
      head: preservedHead,
      baseUrl,
      assetReferences,
    };
    loadingProject = true;
    editor.setComponents(parsed.body.innerHTML);
    editor.setStyle(localStyles.join('\n'));
    applyCanvasDocumentContext();
    const result = window.koDesignerGetProjectData();
    window.setTimeout(() => {
      loadingProject = false;
    }, 100);
    return result;
  };
  window.koDesignerExportHtml = () => {
    const attributes = (items) => items
      .map(([name, value]) => ` ${name}="${String(value).replace(/&/g, '&amp;').replace(/"/g, '&quot;')}"`)
      .join('');
    const head = preservedDocument.head.join('\n');
    const base = preservedDocument.baseUrl
      ? `<base href="${preservedDocument.baseUrl.replace(/&/g, '&amp;').replace(/"/g, '&quot;')}">`
      : '';
    let css = editor.getCss();
    Object.entries(preservedDocument.assetReferences || {}).forEach(
      ([dataUrl, reference]) => {
        css = css.split(dataUrl).join(reference);
      },
    );
    css = css.replace(/<\/style/gi, '<\\/style');
    const charset = /<meta\s+[^>]*charset\s*=/i.test(head)
      ? ''
      : '<meta charset="utf-8">';
    let html = `${preservedDocument.doctype}\n<html${attributes(preservedDocument.htmlAttributes)}>\n<head>\n${charset}\n${base}\n${head}\n<style>\n${css}\n</style>\n</head>\n<body${attributes(preservedDocument.bodyAttributes)}>\n${editor.getHtml()}\n</body>\n</html>`;
    Object.entries(preservedDocument.assetReferences || {}).forEach(
      ([dataUrl, reference]) => {
        html = html.split(dataUrl).join(reference);
      },
    );
    return html;
  };
  window.koDesignerExportProjectHtml = (projectData) => {
    const currentProjectData = editor.getProjectData();
    const currentPreservedDocument = preservedDocument;
    loadingProject = true;
    editor.loadProjectData(projectData);
    preservedDocument = projectData.koDesigner || {
      doctype: '<!DOCTYPE html>',
      htmlAttributes: [],
      bodyAttributes: [],
      head: [],
      baseUrl: '',
      assetReferences: {},
    };
    const html = window.koDesignerExportHtml();
    editor.loadProjectData(currentProjectData);
    preservedDocument = currentPreservedDocument;
    applyCanvasDocumentContext();
    window.setTimeout(() => {
      loadingProject = false;
    }, 100);
    return html;
  };
  window.koDesignerNewProject = () => {
    loadingProject = true;
    editor.setComponents('');
    editor.setStyle([]);
    preservedDocument = {
      doctype: '<!DOCTYPE html>',
      htmlAttributes: [],
      bodyAttributes: [],
      head: [],
      baseUrl: '',
      assetReferences: {},
    };
    const projectData = window.koDesignerGetProjectData();
    window.setTimeout(() => {
      loadingProject = false;
    }, 100);
    return projectData;
  };

  function applyCanvasDocumentContext() {
    const canvasDocument = editor.Canvas.getDocument();
    if (!canvasDocument) return;
    canvasDocument.querySelectorAll('base[data-kd-base], link[data-kd-imported]')
      .forEach((element) => element.remove());
    if (preservedDocument.baseUrl) {
      const base = canvasDocument.createElement('base');
      base.href = preservedDocument.baseUrl;
      base.dataset.kdBase = 'true';
      canvasDocument.head.prepend(base);
    }
    preservedDocument.head.forEach((markup) => {
      const template = canvasDocument.createElement('template');
      template.innerHTML = markup;
      const link = template.content.querySelector('link[rel~="stylesheet"]');
      if (link) {
        link.dataset.kdImported = 'true';
        canvasDocument.head.append(link);
      }
    });
  }

  window.addEventListener('resize', () => editor.refresh());
  new QWebChannel(qt.webChannelTransport, (channel) => {
    designerBridge = channel.objects.koDesignerBridge;
    window.koDesignerReady = true;
    designerBridge.designerReady(JSON.stringify(editor.getProjectData()));
  });
} catch (error) {
  runtimeError.textContent = `KoDesigner initialization failed: ${error.message}`;
  runtimeError.hidden = false;
  console.error(error);
}
