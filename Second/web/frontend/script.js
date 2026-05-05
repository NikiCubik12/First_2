const API_BASE = '/api';

let currentState = {
    dynamicArrayCreated: false,
    dynamicArraySize: 0,
    dynamicArrayValues: [],
    linkedListCreated: false,
    linkedListSize: 0,
    linkedListValues: [],
    sequenceCreated: false,
    sequenceType: 'ArraySequence',
    sequenceSize: 0,
    sequenceValues: [],
    sequenceImpl: 'None'
};

function showMessage(msg, isError = false) {
    const msgDiv = document.getElementById('message-text');
    msgDiv.textContent = isError ? `❌ ${msg}` : `✅ ${msg}`;
    const container = document.getElementById('message-area');
    container.style.borderLeftColor = isError ? '#dc3545' : '#28a745';
}

async function fetchState() {
    try {
        const response = await fetch('/state');
        if (!response.ok) throw new Error('HTTP error: ' + response.status);
        const state = await response.json();
        currentState = state;
        updateUI();
    } catch (error) {
        console.error('Fetch error:', error);
        showMessage('Не удалось соединиться с сервером', true);
    }
}

function updateUI() {
    // Dynamic Array
    const daValues = document.getElementById('da-values');
    if (currentState.dynamicArrayCreated) {
        daValues.textContent = currentState.dynamicArrayValues.length ? 
            `[${currentState.dynamicArrayValues.join(', ')}]` : '[]';
        document.getElementById('da-size').textContent = currentState.dynamicArraySize;
        document.getElementById('da-status').textContent = 'Создан';
        document.getElementById('da-status').style.color = '#28a745';
    } else {
        daValues.textContent = '[пусто]';
        document.getElementById('da-size').textContent = '0';
        document.getElementById('da-status').textContent = 'Не создан';
        document.getElementById('da-status').style.color = '#dc3545';
    }
    
    // Linked List
    const llValues = document.getElementById('ll-values');
    if (currentState.linkedListCreated) {
        llValues.textContent = currentState.linkedListValues.length ? 
            `[${currentState.linkedListValues.join(' → ')}]` : '[]';
        document.getElementById('ll-size').textContent = currentState.linkedListSize;
        document.getElementById('ll-status').textContent = 'Создан';
        document.getElementById('ll-status').style.color = '#28a745';
    } else {
        llValues.textContent = '[пусто]';
        document.getElementById('ll-size').textContent = '0';
        document.getElementById('ll-status').textContent = 'Не создан';
        document.getElementById('ll-status').style.color = '#dc3545';
    }
    
    // Sequence
    const seqValues = document.getElementById('seq-values');
    if (currentState.sequenceCreated) {
        seqValues.textContent = currentState.sequenceValues.length ? 
            `[${currentState.sequenceValues.join(', ')}]` : '[]';
        document.getElementById('seq-size').textContent = currentState.sequenceSize;
        
        let typeDisplay = currentState.sequenceType;
        if (currentState.sequenceType === 'ArraySequence' && currentState.sequenceImpl !== 'None') {
            typeDisplay += ` (${currentState.sequenceImpl === 'Mutable' ? 'изменяемая' : 'неизменяемая'})`;
        } else if (currentState.sequenceType === 'BitSequence') {
            typeDisplay = 'BitSequence (битовая)';
        } else if (currentState.sequenceType === 'ListSequence') {
            typeDisplay = 'ListSequence (связный список)';
        }
        document.getElementById('seq-type-display').textContent = typeDisplay;
    } else {
        seqValues.textContent = '[пусто]';
        document.getElementById('seq-size').textContent = '0';
        document.getElementById('seq-type-display').textContent = 'Не создана';
    }
}

async function apiCall(endpoint, params) {
    try {
        const response = await fetch(endpoint, {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
            body: new URLSearchParams(params)
        });
        const result = await response.json();
        if (result.success) {
            showMessage(result.message);
            await fetchState();
            if (result.value !== undefined) {
                showMessage(`${result.message} (Значение: ${result.value})`);
            }
        } else {
            showMessage(result.message, true);
        }
        return result;
    } catch (error) {
        showMessage('Ошибка запроса: ' + error.message, true);
        return { success: false };
    }
}

// ========== ДИНАМИЧЕСКИЙ МАССИВ ==========
async function daCreate() {
    await apiCall('/api/dynamicarray', { action: 'create' });
}
async function daAppend() {
    const val = document.getElementById('da-append-val').value;
    if (!val) { showMessage('Введите значение', true); return; }
    await apiCall('/api/dynamicarray', { action: 'append', value: val });
    document.getElementById('da-append-val').value = '';
}
async function daPrepend() {
    const val = document.getElementById('da-prepend-val').value;
    if (!val) { showMessage('Введите значение', true); return; }
    await apiCall('/api/dynamicarray', { action: 'prepend', value: val });
    document.getElementById('da-prepend-val').value = '';
}
async function daInsert() {
    const idx = document.getElementById('da-insert-index').value;
    const val = document.getElementById('da-insert-val').value;
    if (!idx || !val) { showMessage('Введите индекс и значение', true); return; }
    await apiCall('/api/dynamicarray', { action: 'insert', index: idx, value: val });
    document.getElementById('da-insert-index').value = '';
    document.getElementById('da-insert-val').value = '';
}
async function daSet() {
    const idx = document.getElementById('da-set-index').value;
    const val = document.getElementById('da-set-val').value;
    if (!idx || !val) { showMessage('Введите индекс и значение', true); return; }
    await apiCall('/api/dynamicarray', { action: 'set', index: idx, value: val });
    document.getElementById('da-set-index').value = '';
    document.getElementById('da-set-val').value = '';
}
async function daGet() {
    const idx = document.getElementById('da-get-index').value;
    if (!idx) { showMessage('Введите индекс', true); return; }
    await apiCall('/api/dynamicarray', { action: 'get', index: idx });
    document.getElementById('da-get-index').value = '';
}
async function daClear() {
    await apiCall('/api/dynamicarray', { action: 'clear' });
}

// ========== СВЯЗНЫЙ СПИСОК ==========
async function llCreate() {
    await apiCall('/api/linkedlist', { action: 'create' });
}
async function llAppend() {
    const val = document.getElementById('ll-append-val').value;
    if (!val) { showMessage('Введите значение', true); return; }
    await apiCall('/api/linkedlist', { action: 'append', value: val });
    document.getElementById('ll-append-val').value = '';
}
async function llPrepend() {
    const val = document.getElementById('ll-prepend-val').value;
    if (!val) { showMessage('Введите значение', true); return; }
    await apiCall('/api/linkedlist', { action: 'prepend', value: val });
    document.getElementById('ll-prepend-val').value = '';
}
async function llInsert() {
    const idx = document.getElementById('ll-insert-index').value;
    const val = document.getElementById('ll-insert-val').value;
    if (!idx || !val) { showMessage('Введите индекс и значение', true); return; }
    await apiCall('/api/linkedlist', { action: 'insert', index: idx, value: val });
    document.getElementById('ll-insert-index').value = '';
    document.getElementById('ll-insert-val').value = '';
}
async function llGet() {
    const idx = document.getElementById('ll-get-index').value;
    if (!idx) { showMessage('Введите индекс', true); return; }
    await apiCall('/api/linkedlist', { action: 'get', index: idx });
    document.getElementById('ll-get-index').value = '';
}
async function llGetFirst() {
    await apiCall('/api/linkedlist', { action: 'getFirst' });
}
async function llGetLast() {
    await apiCall('/api/linkedlist', { action: 'getLast' });
}
async function llClear() {
    await apiCall('/api/linkedlist', { action: 'clear' });
}

// ========== ПОСЛЕДОВАТЕЛЬНОСТИ ==========
function toggleSeqImpl() {
    const seqType = document.getElementById('seq-type').value;
    const implSelect = document.getElementById('seq-impl');
    const implLabel = document.getElementById('seq-impl-label');
    
    if (seqType === 'ArraySequence') {
        implSelect.style.display = 'inline-block';
        implLabel.style.display = 'inline';
    } else {
        implSelect.style.display = 'none';
        implLabel.style.display = 'none';
    }
}

async function seqCreate() {
    const type = document.getElementById('seq-type').value;
    let impl = 'None';
    
    if (type === 'ArraySequence') {
        impl = document.getElementById('seq-impl').value;
    }
    
    await apiCall('/api/sequence', { action: 'create', type, impl });
}
async function seqAppend() {
    const val = document.getElementById('seq-append-val').value;
    if (!val) { showMessage('Введите значение', true); return; }
    await apiCall('/api/sequence', { action: 'append', value: val });
    document.getElementById('seq-append-val').value = '';
}
async function seqPrepend() {
    const val = document.getElementById('seq-prepend-val').value;
    if (!val) { showMessage('Введите значение', true); return; }
    await apiCall('/api/sequence', { action: 'prepend', value: val });
    document.getElementById('seq-prepend-val').value = '';
}
async function seqInsert() {
    const idx = document.getElementById('seq-insert-index').value;
    const val = document.getElementById('seq-insert-val').value;
    if (!idx || !val) { showMessage('Введите индекс и значение', true); return; }
    await apiCall('/api/sequence', { action: 'insert', index: idx, value: val });
    document.getElementById('seq-insert-index').value = '';
    document.getElementById('seq-insert-val').value = '';
}
async function seqGet() {
    const idx = document.getElementById('seq-get-index').value;
    if (!idx) { showMessage('Введите индекс', true); return; }
    await apiCall('/api/sequence', { action: 'get', index: idx });
    document.getElementById('seq-get-index').value = '';
}
async function seqGetFirst() {
    await apiCall('/api/sequence', { action: 'getFirst' });
}
async function seqGetLast() {
    await apiCall('/api/sequence', { action: 'getLast' });
}

// ========== ПЕРЕКЛЮЧЕНИЕ ВКЛАДОК ==========
document.querySelectorAll('.tab-btn').forEach(btn => {
    btn.addEventListener('click', () => {
        const tab = btn.dataset.tab;
        document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
        document.querySelectorAll('.tab-content').forEach(c => c.classList.remove('active'));
        btn.classList.add('active');
        document.getElementById(`${tab}-tab`).classList.add('active');
        fetchState();
    });
});

// ========== НАСТРОЙКА ВЫБОРА ТИПА ПОСЛЕДОВАТЕЛЬНОСТИ ==========
document.getElementById('seq-type').addEventListener('change', toggleSeqImpl);

// При загрузке страницы
document.addEventListener('DOMContentLoaded', function() {
    toggleSeqImpl();
});

// ========== ПОДДЕРЖКА КЛАВИШИ ENTER ==========
document.querySelectorAll('input').forEach(input => {
    input.addEventListener('keypress', (e) => {
        if (e.key === 'Enter') {
            const parent = input.closest('.operation');
            const btn = parent?.querySelector('button');
            if (btn) btn.click();
        }
    });
});

// ========== ЗАПУСК ==========
fetchState();
setInterval(fetchState, 3000);