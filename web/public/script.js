document.getElementById('btn-encode').addEventListener('click', async () => {
    const n = parseInt(document.getElementById('n-val').value);
    const k = parseInt(document.getElementById('k-val').value);
    const g = document.getElementById('g-val').value.trim();
    const m = document.getElementById('m-val').value.trim();
    const errorMsg = document.getElementById('error-msg');
    const resultContainer = document.getElementById('result-container');

    errorMsg.classList.add('hidden');
    resultContainer.classList.add('hidden');

    if (!n || !k || !g || !m) {
        showError('Vui lòng nhập đầy đủ thông số!');
        return;
    }

    try {
        const response = await fetch('/api/encode', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ n, k, g, m })
        });

        const data = await response.json();

        if (!response.ok) {
            throw new Error(data.error || 'Có lỗi xảy ra');
        }

        renderTable(data);
        renderSolution(data);
        renderCircuit(data);

        resultContainer.classList.remove('hidden');
    } catch (err) {
        showError(err.message);
    }
});

function showError(msg) {
    const errorMsg = document.getElementById('error-msg');
    errorMsg.textContent = msg;
    errorMsg.classList.remove('hidden');
}

function renderTable(data) {
    const table = document.getElementById('state-table');
    const thead = table.querySelector('thead');
    const tbody = table.querySelector('tbody');

    // Header
    let headerHtml = '<tr><th>Nhịp</th><th>Khóa K1, K2</th><th>Input (m)</th><th>Feedback</th>';
    for (let i = data.r - 1; i >= 0; i--) {
        headerHtml += `<th>R${i}</th>`;
    }
    headerHtml += '</tr>';
    thead.innerHTML = headerHtml;

    // Body
    let bodyHtml = '';
    data.steps.forEach(step => {
        const switchClass = step.switch === 'ON' ? 'switch-on' : 'switch-off';
        const inputStr = step.switch === 'ON' ? step.input : '-';
        const feedbackStr = step.switch === 'ON' ? step.feedback : '-';
        
        bodyHtml += `<tr>
            <td>${step.step}</td>
            <td class="${switchClass}">${step.switch}</td>
            <td>${inputStr}</td>
            <td>${feedbackStr}</td>`;
        
        // Reverse order of regs for display (Rr-1 to R0)
        for (let i = data.r - 1; i >= 0; i--) {
            bodyHtml += `<td>${step.regs[i]}</td>`;
        }
        bodyHtml += '</tr>';
    });
    tbody.innerHTML = bodyHtml;
}

function renderSolution(data) {
    const container = document.getElementById('step-by-step-text');
    let html = '';

    html += `<p><strong>Cho:</strong></p>
    <ul>
        <li>Mã vòng C(${data.n}, ${data.k}) &rArr; Số thanh ghi $r = n - k = ${data.r}$</li>
        <li>Đa thức sinh <span class="math-formula">g(x) = ${formatPoly(data.g_str)}</span> (Dạng bit: ${data.g_str})</li>
        <li>Bản tin <span class="math-formula">m(x) = ${formatPoly(data.m_str)}</span> (Dạng bit: ${data.m_str})</li>
    </ul>`;

    html += `<p><strong>Thuật toán thực hiện qua ${data.n} nhịp:</strong></p>`;
    
    html += `<div class="step-block">
        <h4>Giai đoạn 1: Khóa K1 đóng, K2 mở (Nhịp 1 đến ${data.k})</h4>
        <p>Bản tin được đưa vào bộ mã hóa và đồng thời truyền ra kênh truyền. Phản hồi được tính bằng: <code>Feedback = Input &oplus; R${data.r - 1}</code></p>`;
    
    // Simulate initial state
    let prevRegs = Array(data.r).fill(0);
    
    for (let i = 0; i < data.k; i++) {
        const step = data.steps[i];
        html += `<p><strong>Nhịp ${step.step}:</strong> Input = ${step.input}. 
                 Feedback = ${step.input} &oplus; ${prevRegs[data.r - 1]} = ${step.feedback}.<br>
                 Dịch bit và cộng XOR: Trạng thái thanh ghi mới là [${[...step.regs].reverse().join(', ')}] (Từ R${data.r - 1} &rarr; R0).</p>`;
        prevRegs = step.regs;
    }
    html += `</div>`;

    if (data.n > data.k) {
        html += `<div class="step-block">
            <h4>Giai đoạn 2: Khóa K1 mở, K2 đóng (Nhịp ${data.k + 1} đến ${data.n})</h4>
            <p>Ngắt đường phản hồi (Feedback = 0). Các thanh ghi chỉ dịch phải (Shift Right) để đẩy phần dư ra kênh truyền.</p>`;
        
        for (let i = data.k; i < data.n; i++) {
            const step = data.steps[i];
            html += `<p><strong>Nhịp ${step.step}:</strong> Dịch bit. Trạng thái thanh ghi mới: [${[...step.regs].reverse().join(', ')}]. Bit xuất ra kênh: ${prevRegs[data.r - 1]}.</p>`;
            prevRegs = step.regs;
        }
        html += `</div>`;
    }

    html += `<div class="final-result">
        <p><strong>Kết quả:</strong></p>
        <p>Từ mã hệ thống thu được là: <strong>${data.codeword.join('')}</strong></p>
        <p>Dạng đa thức: <span class="math-formula">c(x) = ${formatPoly(data.codeword.join(''))}</span></p>
    </div>`;

    container.innerHTML = html;
}

function formatPoly(bitString) {
    let terms = [];
    for (let i = 0; i < bitString.length; i++) {
        if (bitString[i] === '1') {
            if (i === 0) terms.push('1');
            else if (i === 1) terms.push('x');
            else terms.push(`x^${i}`);
        }
    }
    return terms.length ? terms.join(' + ') : '0';
}

function renderCircuit(data) {
    const container = document.getElementById('circuit-diagram');
    const r = data.r;
    const g = data.g_str; // g_str goes from g_0 to g_r (length r+1). Note: g_0 is g[0], g_r is g[r].
    
    // SVG Dimensions
    const regWidth = 60;
    const regHeight = 40;
    const xSpacing = 80;
    const ySpacing = 80;
    
    const svgWidth = (r + 2) * xSpacing + 100;
    const svgHeight = 200;
    
    let svgHtml = `<svg class="circuit-svg" viewBox="0 0 ${svgWidth} ${svgHeight}" xmlns="http://www.w3.org/2000/svg">
        <style>
            .line { stroke: var(--primary); stroke-width: 2; fill: none; }
            .reg { fill: white; stroke: var(--primary); stroke-width: 2; }
            .text { font-family: 'Fira Code', monospace; font-size: 14px; fill: var(--text-main); text-anchor: middle; dominant-baseline: middle; }
            .xor { fill: white; stroke: var(--secondary); stroke-width: 2; }
            .xor-cross { stroke: var(--secondary); stroke-width: 2; }
            .dot { fill: var(--primary); }
            .switch { stroke: var(--error); stroke-width: 3; stroke-linecap: round; }
            .label { font-size: 12px; fill: var(--text-muted); }
        </style>
        
        <!-- Definions for arrowhead -->
        <defs>
            <marker id="arrow" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="6" markerHeight="6" orient="auto-start-reverse">
                <path d="M 0 0 L 10 5 L 0 10 z" fill="var(--primary)" />
            </marker>
        </defs>
    `;

    // Coordinates
    const startX = 50;
    const yRegs = 100;
    const yFeedback = yRegs - ySpacing;

    // Draw Input Line & Feedback XOR
    const inputX = svgWidth - 100;
    svgHtml += `
        <!-- Input line -->
        <line x1="${inputX + 50}" y1="${yRegs}" x2="${inputX}" y2="${yRegs}" class="line" marker-end="url(#arrow)" />
        <text x="${inputX + 25}" y="${yRegs - 15}" class="text">Input</text>
        
        <!-- Feedback XOR -->
        <circle cx="${inputX}" cy="${yRegs}" r="12" class="xor" />
        <line x1="${inputX - 8}" y1="${yRegs}" x2="${inputX + 8}" y2="${yRegs}" class="xor-cross" />
        <line x1="${inputX}" y1="${yRegs - 8}" x2="${inputX}" y2="${yRegs + 8}" class="xor-cross" />
        
        <!-- Feedback Line -->
        <line x1="${inputX}" y1="${yRegs - 12}" x2="${inputX}" y2="${yFeedback}" class="line" />
        <line x1="${inputX}" y1="${yFeedback}" x2="${startX}" y2="${yFeedback}" class="line" marker-end="url(#arrow)" />
        <text x="${(inputX + startX)/2}" y="${yFeedback - 10}" class="text">Feedback (K1)</text>
    `;

    let currentX = startX;

    // Draw Registers and intermediate XORs
    for (let i = 0; i < r; i++) {
        // Line down from feedback if g_{r-i} == 1 (Wait, let's map g to XORs)
        // g_str has length r+1 (from g_0 to g_r). 
        // g_0 is at index 0. The connection to R0 input comes from feedback AND g_r. Wait, C++ uses g.getBit(r-i).
        // Let's trace C++: 
        // nextRegs[0] = feedback * g.getBit(r); -> so feedback goes directly into R0 (since g_r is always 1)
        // nextRegs[i] = regs[i - 1] + (feedback * g.getBit(r - i)); -> XOR before R_i if g_{r-i} == 1
        
        // Let's index g_str from left (g_0) to right (g_r).
        // g_str[0] = g_0, g_str[r] = g_r.
        // The feedback multiplier before R_i is g_{r-i}.
        // So for R_0 (i=0), we use g_str[r].
        // For R_1 (i=1), we use g_str[r-1].
        
        // Input to this stage
        if (i === 0) {
            // Into R0
            svgHtml += `
                <line x1="${currentX}" y1="${yFeedback}" x2="${currentX}" y2="${yRegs}" class="line" marker-end="url(#arrow)" />
            `;
        } else {
            // Connection from previous R
            const prevRegRight = currentX - xSpacing + regWidth;
            const bit_g = data.g_str[data.g_str.length - 1 - i]; // g_{r-i}
            
            if (bit_g === '1') {
                // Draw XOR
                const xorX = prevRegRight + (xSpacing - regWidth) / 2;
                svgHtml += `
                    <line x1="${prevRegRight}" y1="${yRegs}" x2="${xorX - 12}" y2="${yRegs}" class="line" marker-end="url(#arrow)" />
                    
                    <!-- XOR -->
                    <circle cx="${xorX}" cy="${yRegs}" r="12" class="xor" />
                    <line x1="${xorX - 8}" y1="${yRegs}" x2="${xorX + 8}" y2="${yRegs}" class="xor-cross" />
                    <line x1="${xorX}" y1="${yRegs - 8}" x2="${xorX}" y2="${yRegs + 8}" class="xor-cross" />
                    
                    <line x1="${xorX + 12}" y1="${yRegs}" x2="${currentX}" y2="${yRegs}" class="line" marker-end="url(#arrow)" />
                    
                    <!-- Feedback connection -->
                    <circle cx="${xorX}" cy="${yFeedback}" r="4" class="dot" />
                    <line x1="${xorX}" y1="${yFeedback}" x2="${xorX}" y2="${yRegs - 12}" class="line" marker-end="url(#arrow)" />
                `;
            } else {
                // Direct line
                svgHtml += `
                    <line x1="${prevRegRight}" y1="${yRegs}" x2="${currentX}" y2="${yRegs}" class="line" marker-end="url(#arrow)" />
                `;
            }
        }

        // Draw Register
        svgHtml += `
            <rect x="${currentX}" y="${yRegs - regHeight/2}" width="${regWidth}" height="${regHeight}" rx="4" class="reg" />
            <text x="${currentX + regWidth/2}" y="${yRegs}" class="text">R${i}</text>
        `;

        currentX += xSpacing;
    }

    // Connect R_{r-1} to feedback XOR
    const lastRegRight = currentX - xSpacing + regWidth;
    svgHtml += `
        <line x1="${lastRegRight}" y1="${yRegs}" x2="${inputX - 12}" y2="${yRegs}" class="line" marker-end="url(#arrow)" />
    `;

    svgHtml += `</svg>`;
    container.innerHTML = svgHtml;
}
